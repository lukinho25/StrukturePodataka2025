#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Sve varijable, funkcije piši na engleskom.

Upute za kod:

struct St* Position;

td struct St{
    ime
    prez
    god
    Position next;
}

A) 1) newPerson = alokacija tipa Position
    2) strcpy: name = " ":, next = NULL;
    3) dohvatiti Head.next
        NEWpERSON->next=Head.next;
        Head.next=newPerson;
- u mainu:
Person Head  {  .name = " ",
        .next = NULL;}

B) for(q=Head->next; q != NULL, q = q->next)

C) 1) alokacija newPerson
    2) strcpy
3)pronaæi zadnji -> isti for, ali q->next != NULL

D) i E) (Obavezno dvije razlicite funkcije)
    q - Head
    temp = q->next;
-provjeraPrezimena
q->next=temp->next
free(temp)
*/

struct St;
typedef struct St* Position;

typedef struct St {
    char ime[50];
    char prezime[50];
    int godRodenja;
    Position next;
} Person;

Position createPerson(char* ime, char* prezime, int godRodenja) {
    Position newPerson = (Position)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Greska pri alokaciji memorije!\n");
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

Position findBySurname(Position head, char* prezime) {
    Position q = head->next;

    while (q != NULL) {
        if (strcmp(q->prezime, prezime) == 0)
            return q;
        q = q->next;
    }

    return NULL;
}

void deleteBySurname(Position head, char* prezime) {
    Position q = head;
    Position temp = q->next;

    while (temp != NULL && strcmp(temp->prezime, prezime) != 0) {
        q = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Osoba s prezimenom %s nije pronadena.\n", prezime);
        return;
    }

    q->next = temp->next;
    free(temp);
    printf("Osoba s prezimenom %s uspjesno obrisana.\n", prezime);
}

int main() {
    Person Head = { .ime = " ", .next = NULL };

    insertAtBeginning(&Head, "Marko", "Maric", 1988);
    insertAtEnd(&Head, "Ana", "Anic", 1995);
    insertAtBeginning(&Head, "Ivan", "Ivic", 1990);

    printf("Trenutna lista osoba:\n");
    printList(&Head);

    printf("\nPretraga po prezimenu 'Anic':\n");
    Position found = findBySurname(&Head, "Anic");
    if (found)
        printf("Pronadena osoba: %s %s (%d)\n", found->ime, found->prezime, found->godRodenja);
    else
        printf("Osoba s tim prezimenom nije pronadena.\n");

    printf("\nBrisanje osobe s prezimenom 'Ivic':\n");
    deleteBySurname(&Head, "Ivic");

    printf("\nLista nakon brisanja:\n");
    printList(&Head);

    return 0;
}
