#include <stdio.h>

/*
Head
firstPoly
secondPoly

Head = {.coeff = 0, .pow = 0, .next = NULL}
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
-buffer
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











int main() {


return 0;

}