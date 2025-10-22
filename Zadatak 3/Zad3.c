#include <stdio.h>

/*

A) alokacija
-inicijalizirati
-petlja - strcmp(q->lastName, lastName) == 0;
-newPerson->next=q->next
-q->next=newPerson

B) alokacija
-inicijalizirati {Position prev = head;, q = prev->next}
-petlja - strcmp(q->lastName, lastName) == 0;

prev=q
q=q->next

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

