#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*

A) alokacija
-inicijalizirati
-petlja - strcmp(q->lastName, lastName) == 0;
-newPerson->next=q->next
-q->next=newPerson

B) alokacija
-inicijalizirati {Position prev = head;, q = prev->next}
-petlja - strcmp(q->lastName, lastName) == 0;

 

prev->next = newPerson
newPerson->next = q

-newPerson->next=q->next
-q->next=newPerson

C) q = head, p = q->next, temp, sorted = NULL (tip Position)

- provjera prazne liste
strcmp(p->lastN, p->next->lastN) > 0

q = q->next
p = p->next

temp = p->next
q->next = temp
p->next = temp->next
temp->next = p

if(p->next==sorted)
sorted = p


D) otvorit
fprintf
zatvorit

E) otvorit
fscanf
zatvorit
*/

struct St;
typedef struct St* Position;

typedef struct St {
    char ime[50];
    char prezime[50];
    int godRodenja;
    Position next;
} Person;

Position createPerson(char* ime, char* prezime, int godRodenja);
void insertAtBeginning(Position head, char* ime, char* prezime, int godRodenja);
void printList(Position head);
void insertAtEnd(Position head, char* ime, char* prezime, int godRodenja);
void insertAfter(Position head, char* ime, char* prezime, int godRodenja, char* prezimeOsobe);
void insertBefore(Position head, char* ime, char* prezime, int godRodenja, char* prezimeOsobe);
void sortiranje(Position head);
//void writeFile(Position head);    zakomentirano radi readFile funkcije
void readFile(Position head);



int main() {
    Person Head = { .ime = " ", .next = NULL };

    insertAtBeginning(&Head, "Marko", "Maric", 1988);
	insertAtEnd(&Head, "Ana", "Anic", 1995);                    //vec unaprijed definirane osobe radi jednostavnosti testiranja
    insertAtBeginning(&Head, "Ivan", "Ivic", 1990);

    printf("Current list:\n");
    printList(&Head);

    printf("\nUnesite prezime osobe iza koje zelite dodati novu osobu: 'Ivic'\n");
    insertAfter(&Head, "Roko", "Rokic", 2005, "Ivic");
    printf("\nNew list:\n");
    printList(&Head);

    printf("\nUnesite prezime osobe ispred koje zelite dodati novu osobu: 'Rokic'\n"); 
    insertBefore(&Head, "Luka", "Lukic", 1998, "Rokic");
    printf("\nNew list:\n");
    printList(&Head);

	printf("\nSorted list by last name:\n");
	sortiranje(&Head);
	printList(&Head);

	//writeFile(&Head);
	readFile(&Head);

	printf("\nFinal list from file:\n");
	printList(&Head);

    return 0;
}

Position createPerson(char* ime, char* prezime, int godRodenja) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Allocation error!\n");
        return NULL;
    }

    strcpy(newPerson->ime, ime);
    strcpy(newPerson->prezime, prezime);
    newPerson->godRodenja = godRodenja;
    newPerson->next = NULL;

    return newPerson;
}

void insertAtBeginning(Position head, char* ime, char* prezime, int godRodenja) {
    Position newPerson = createPerson(ime, prezime, godRodenja);
    newPerson->next = head->next;
    head->next = newPerson;
}

void printList(Position head) {
    Position q = head->next;
    while (q != NULL) {
        printf("%s %s (%d)\n", q->ime, q->prezime, q->godRodenja);
        q = q->next;
    }
}

void insertAtEnd(Position head, char* ime, char* prezime, int godRodenja) {
    Position newPerson = createPerson(ime, prezime, godRodenja);
    Position q = head;

    while (q->next != NULL)
        q = q->next;

    q->next = newPerson;
}

void insertAfter(Position head, char* ime, char* prezime, int godRodenja, char* prezimeOsobe) {
	Position newPerson = createPerson(ime, prezime, godRodenja);
    Position q = head->next;

    while (q != NULL && strcmp(q->prezime, prezimeOsobe) != 0) {
        q = q->next;
    }
    if (q != NULL) {
        
        newPerson->next = q->next;
        q->next = newPerson;
    }
    else {
        printf("Person with last name %s not found!\n", prezimeOsobe);
        free(newPerson);
    }

}

void insertBefore(Position head, char* ime, char* prezime, int godRodenja, char* prezimeOsobe) {
    Position newPerson = createPerson(ime, prezime, godRodenja);
    Position prev = head;
    Position q = prev->next;

    while (q != NULL && strcmp(q->prezime, prezimeOsobe) != 0) {
        prev = q;
        q = q->next;
    }
    if (q != NULL) {
        prev->next = newPerson;
        newPerson->next = q;
    }
    else {
        printf("Person with last name %s not found!\n", prezimeOsobe);
        free(newPerson);
    }

}

void sortiranje(Position head) {
    Position sorted = NULL;
    Position q, p, temp;
    if (head->next == NULL) {
        printf("The list is empty.");
        return;
    }

    while (head->next != sorted) {
        q = head;
        p = q->next;
        while (p->next != sorted) {
            if (strcmp(p->prezime, p->next->prezime) > 0) {

                temp = p->next;
                q->next = temp;
                p->next = temp->next;
                temp->next = p;
                p = temp;
            }
            q = p;
            p = p->next;
        }
        sorted = p;
    }
}


/*void writeFile(Position head) {
    FILE* fp = fopen("persons.txt", "w");
    if (!fp) {
        printf("Error opening file for writing!\n");
        return;
    }
    Position q = head->next;
    while (q != NULL) {
        fprintf(fp, "%s %s (%d)\n", q->ime, q->prezime, q->godRodenja);
        q = q->next;
    }
    fclose(fp);
}*/

void readFile(Position head) {
    FILE* fp = fopen("persons.txt", "r");
    if (!fp) {
        printf("Error opening file for reading!\n");
        return;
    }

    Position temp;
    while (head->next != NULL) {
		temp = head->next;          //oslobadanje postojece liste prije ucitavanja iz datoteke
        head->next = temp->next;
        free(temp);
    }
    char ime[50], prezime[50];
    int godRodenja;
    while (fscanf(fp, "%s %s (%d)\n", ime, prezime, &godRodenja) != EOF) {
        insertAtEnd(head, ime, prezime, godRodenja);
    }
    fclose(fp);
}
    
