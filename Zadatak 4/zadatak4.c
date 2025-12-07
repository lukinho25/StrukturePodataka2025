#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Head
firstPoly
secondPoly

poly Head = {.coeff = 0, .pow = 0, .next = NULL}
firstPoly = Head;
secondPoly = Head;
q = &Head;

td poly * Position;
td struct poly {
    int coeff;
    int pow;
    Position next;
    }Poly;

otvoriti dat
-buffer, nakon svega oslobodit buffer
if(sscanf(buffer, "%d %d %n", &coeff, &pow, &n) != 2) {
    
-newEl = Null;
p = &firstPoly;
if(p->next == Null)
    - alokacija
    newEl -> next = p -> next
    p -> next = newEl

    prev = p
    p = p -> next

    dalje koristi cijeli puni naziv npr &coefficient i &power

    if(p->next == NULL && p->pow > power) {
     
if(p != null && p->pow < power)
    -alokacija
    newEl -> next = p;
    prev -> next = newEl;

if(p != null && p->pow == power);
    -   alokacija

ili izbrisat jednog clana ili procitati novi koeficijent i dodati na postojeći

p->coeff += newEl -> coefficient
free(newEl)

if(p->coeff == 0)
    prev -> next = p -> next
    p -> next = Null
    free(p)

    
*/

typedef struct poly* Pozicija;

typedef struct poly {
    int coeff;
    int pow;
    Pozicija next;
} Poly;

Pozicija createTerm(int coeff, int pow) {
    Pozicija newEl = malloc(sizeof(Poly));
    if (!newEl) return NULL;
    newEl->coeff = coeff;
    newEl->pow = pow;
    newEl->next = NULL;
    return newEl;
}

void insertSorted(Pozicija head, int coeff, int pow) {
    Pozicija prev = head;
    Pozicija p = head->next;

    while (p != NULL && p->pow > pow) {
        prev = p;
        p = p->next;
    }
    if (p != NULL && p->pow == pow) {
        p->coeff += coeff;
        if (p->coeff == 0) {
            prev->next = p->next;
            free(p);
        }
        return;
    }
    Pozicija newEl = createTerm(coeff, pow);
    newEl->next = p;
    prev->next = newEl;
}

void readPoly(FILE* f, Pozicija head) {
    char buffer[1024];
    int coeff, pow, n, pos = 0;

    if (!fgets(buffer, sizeof(buffer), f)) return;

    while (sscanf(buffer + pos, "%d %d %n", &coeff, &pow, &n) == 2) {
        insertSorted(head, coeff, pow);
        pos += n;
    }
}

void printPoly(Pozicija head) {
    Pozicija p = head->next;
    if (!p) {
        printf("0\n");
        return;
    }
    while (p != NULL) {
        printf("%dX^%d ", p->coeff, p->pow);
        p = p->next;
    }
    printf("\n");
}

void addPoly(Pozicija a, Pozicija b, Pozicija result) {
    Pozicija pa = a->next;
    Pozicija pb = b->next;

    while (pa != NULL) {
        insertSorted(result, pa->coeff, pa->pow);
        pa = pa->next;
    }
    while (pb != NULL) {
        insertSorted(result, pb->coeff, pb->pow);
        pb = pb->next;
    }
}

void mulPoly(Pozicija a, Pozicija b, Pozicija result) {
    for (Pozicija pa = a->next; pa != NULL; pa = pa->next) {
        for (Pozicija pb = b->next; pb != NULL; pb = pb->next) {
            insertSorted(result, pa->coeff * pb->coeff, pa->pow + pb->pow);
        }
    }
}

int main() {
    FILE* f = fopen("polinomi.txt", "r");
    if (!f) {
        printf("Ne mogu otvoriti datoteku!\n");
        return -1;
    }

    Poly first = {0, 0, NULL};
    Poly second = {0, 0, NULL};
    Poly sum = {0, 0, NULL};
    Poly product = {0, 0, NULL};

    readPoly(f, &first);
    readPoly(f, &second);

    fclose(f);

    printf("Prvi polinom: ");
    printPoly(&first);
    printf("Drugi polinom: ");
    printPoly(&second);

    addPoly(&first, &second, &sum);
    mulPoly(&first, &second, &product);

    printf("\nZbroj:     ");
    printPoly(&sum);
    printf("Umnožak:   ");
    printPoly(&product);

    return 0;
}


