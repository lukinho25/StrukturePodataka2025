#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
1) td struct -postfix* Position;
td struct -postfix {
    double number;
    Poistion next;
}postfix;    

head = {.number = 0, .next = NULL};

2) ucitat datoteku
pokraniti podatke 
zatvoriti datoteku

oslobodit buffer i listu 

3) a) broj? 
b) operator?
c) trash?

a) Push(Position head, double num);
- alocirati newEl tipa Position
newEl->number = num;
newEl->next = NULL;

q = head;
newEl->next = q->next;
q->next = newEl;

b) obavimo operaciju
rezultat
Pop(Position first, Position second);
first = q->next;
second = first->next;
{q->next = second->next;
first->next = NULL;
free(first);}
second->next = NULL;
free(second);

Push(q, rezultat);
npr. 4 5 * 4 + 11 / 2 - 

*/

// Definicija strukture za elemente stoga
typedef struct Postfix* Position;
typedef struct Postfix {
    double number;
    Position next;
}Postfix;

// Funkcija za stvaranje novog elementa stoga
Position CreateNewElement(double num) {
    Position newEl = NULL;
    newEl = (Position)malloc(sizeof(Postfix));
    if (!newEl) {
        perror("Memory allocation failed!");
        return NULL;
    }
    newEl->number = num;
    newEl->next = NULL;
    return newEl;
}

// Funkcija za dodavanje elementa na stog
void Push(Position head, double num) {
    Position newEl = CreateNewElement(num);
    if (!newEl) {
        return;
    }
    Position q = head;
    newEl->next = q->next;
    q->next = newEl;
}


// Funkcija za uklanjanje elementa sa stoga
double Pop(Position head) {
    Position first = head->next;
    if (!first) {
        fprintf(stderr, "Stack underflow!");
        return 0; 
    }
    double num = first->number;
    head->next = first->next;
    free(first); // Oslobađanje memorije uklonjenog elementa
    return num;
}

// Funkcija za izvođenje operacije nad dva broja sa stoga
int performOperation(Position head, char operator) {
    double firstNum, secondNum, result;
    secondNum = Pop(head); //Drugi broj je na vrhu stoga
    firstNum = Pop(head); //Prvi broj je ispod njega

    switch (operator) {
        case '+':
            result = firstNum + secondNum;
            break;
        case '-':
            result = firstNum - secondNum;
            break;
        case '*':
            result = firstNum * secondNum;
            break;
        case '/':
            if (secondNum == 0) {
                fprintf(stderr, "Division by zero!");
                return -1;
            }
            result = firstNum / secondNum;
            break;
        default:
            fprintf(stderr, "Unknown operator: %c", operator);
            return -1;
    }
    Push(head, result); 
    return 0;
}

// Funkcija za čitanje datoteke i evaluaciju postfix izraza
int readFile(const char* filename, Position head) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file!");
        return -1;
    }

    char buffer[256];
    while (fgets(buffer, sizeof(buffer), file)) {
        char* token = strtok(buffer, " \n");
        while (token) {
            char* endptr;
            double num = strtod(token, &endptr); // Pokušaj konverzije u broj
            if (endptr != token) { // Uspješna konverzija
                Push(head, num);
            } else if (strlen(token) == 1 && strchr("+-*/", token[0])) { // Provjera je li token operator
                if (performOperation(head, token[0]) != 0) { 
                    fclose(file);
                    return -1;
                }
            } else {
                fprintf(stderr, "Invalid token: %s", token);
                fclose(file);
                return -1;
            }
            token = strtok(NULL, " \n");
        }
    }

    fclose(file);
    return 0;
}

// Funkcija za oslobađanje memorije stoga
void freeStack(Position head) {
    Position current = head->next;
    Position temp;
    while (current) {
        temp = current;
        current = current->next;
        free(temp);
    }
}

int main() {
    Position head = CreateNewElement(0); //Dummy head element
    //Position head = { .number = 0, .next = NULL };
    if (!head) {
        return -1;
    }

    if (readFile("postfix.txt", head) != 0) { //Ako dođe do greške pri čitanju datoteke ili evaluaciji osloboditi memoriju i izaći
        freeStack(head);
        free(head);
        return -1;
    }

    if (head->next && head->next->next == NULL) { // Provjera sadrži li stog točno jedan element
        printf("Result: %lf\n", head->next->number);
    } else {
        fprintf(stderr, "Stack should contain exactly one element after evaluation.");
    }

    freeStack(head);
    free(head);
    return 0;
}
