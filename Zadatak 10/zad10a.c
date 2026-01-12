/* 
10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država. Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države. Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu. Svaki čvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo država sortirano po nazivu. Svaki čvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem
tastature pretragu gradova određene države koji imaju broj stanovnika veći od unosa na
tastaturi.

struct country{
    char countryName[20];
    Position next;
    treePos root;
}

struct city{
    int population;
    char cityName[20];
    treePos left;
    treePos right;
}

-Head na null
-strtok, prvi token se pohranjuje u countryName, a drugi za citanje nove datoteke (npr. Francuska fr.txt)

p->next = NULL
alokacija
newEl->next = p->next
p->next = newEl

b) p->next != NULL i p->next->countryName > countryName
isto kao a)

c) p->next != NULL i p->next->countryName < countryName
p = p->next

ubacivanje gradova
a) root == NULL
ako je , createNode(root, population)
alokacija
b) population > city->population
    root->right = Insert(root->right, population)
c) population < city->population
    root->left = Insert(root->left, population)

za pod b) zadatka formirat stablo drzava
    potpuno ista stvar, pazit na pokazivace i neke od uvjeta

za ovo na kraju zadatka
    p != NULL
    printf(countryName
    printTree(p->root
    root != NULL
    printTree(root->left)
    printf(root->cityName)
    printTree(root->right)

while(p)
    usporedujemo drzave strcmp
    pretrazujemo gradove
p = p->next

searchTree(root, population)
if(root == NULL) return NULL
else if(root->population < population)
   printf(root->cityName)
else  
   searchTree(root->right, population)
   searchTree(root->left, population)

oslobadanje memorije
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct city* CityPos;
typedef struct country* CountryPos;

struct city {
    char name[20];
    int population;
    CityPos left;
    CityPos right;
};

struct country {
    char name[20];
    CityPos root;
    CountryPos next;
};



CityPos CreateCity(char* name, int population) {
    CityPos c = malloc(sizeof(struct city));
    strcpy(c->name, name);
    c->population = population;
    c->left = c->right = NULL;
    return c;
}

CityPos InsertCity(CityPos root, char* name, int population) {
    if (!root)
        return CreateCity(name, population);

    if (population < root->population)
        root->left = InsertCity(root->left, name, population);
    else if (population > root->population)
        root->right = InsertCity(root->right, name, population);
    else if (strcmp(name, root->name) < 0)
        root->left = InsertCity(root->left, name, population);
    else
        root->right = InsertCity(root->right, name, population);

    return root;
}

void PrintCities(CityPos root) {
    if (!root) return;
    PrintCities(root->left);
    printf("   %s (%d)\n", root->name, root->population);
    PrintCities(root->right);
}

void FreeCityTree(CityPos root) {
    if (!root) return;
    FreeCityTree(root->left);
    FreeCityTree(root->right);
    free(root);
}



CountryPos CreateCountry(char* name) {
    CountryPos c = malloc(sizeof(struct country));
    strcpy(c->name, name);
    c->root = NULL;
    c->next = NULL;
    return c;
}

void ReadCities(CountryPos country, char* fileName) {
    FILE* fp = fopen(fileName, "r");
    char city[20];
    int pop;

    while (fscanf(fp, " %[^,],%d", city, &pop) == 2)
        country->root = InsertCity(country->root, city, pop);

    fclose(fp);
}

void InsertCountrySorted(CountryPos head, char* name, char* fileName) {
    CountryPos p = head;

    while (p->next && strcmp(p->next->name, name) < 0)
        p = p->next;

    CountryPos newC = CreateCountry(name);
    ReadCities(newC, fileName);

    newC->next = p->next;
    p->next = newC;
}

void PrintCountries(CountryPos head) {
    CountryPos p = head->next;

    while (p) {
        printf("%s\n", p->name);
        PrintCities(p->root);
        p = p->next;
    }
}

void FreeCountries(CountryPos head) {
    CountryPos tmp;
    while (head->next) {
        tmp = head->next;
        head->next = tmp->next;
        FreeCityTree(tmp->root);
        free(tmp);
    }
}



void SearchCities(CityPos root, int minPop) {
    if (!root) return;
    if (root->population > minPop)
        printf("   %s (%d)\n", root->name, root->population);
    SearchCities(root->left, minPop);
    SearchCities(root->right, minPop);
}

void FindCountry(CountryPos head) {
    char name[20];
    int minPop;
    CountryPos p;

    printf("Unesi drzavu: ");
    scanf("%s", name);
    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    p = head->next;
    while (p && strcmp(p->name, name))
        p = p->next;

    if (p)
        SearchCities(p->root, minPop);
    else
        printf("Drzava ne postoji.\n");
}



int main(void) {
    struct country head = { "", NULL, NULL };

    FILE* fp = fopen("drzave.txt", "r");
    char country[20], file[20];

    while (fscanf(fp, "%s %s", country, file) == 2)
        InsertCountrySorted(&head, country, file);

    fclose(fp);

    PrintCountries(&head);
    FindCountry(&head);
    FreeCountries(&head);

    return 0;
}
