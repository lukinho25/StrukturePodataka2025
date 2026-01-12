#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct city* CityPos;
typedef struct country* CountryPos;

struct city {
    char name[20];
    int population;
    CityPos next;
};

struct country {
    char name[20];
    CityPos head;
    CountryPos left;
    CountryPos right;
};



CityPos CreateCity(char* name, int population) {
    CityPos c = malloc(sizeof(struct city));
    strcpy(c->name, name);
    c->population = population;
    c->next = NULL;
    return c;
}

void InsertCitySorted(CityPos head, char* name, int population) {
    CityPos p = head;

    while (p->next &&
          (p->next->population < population ||
          (p->next->population == population &&
           strcmp(p->next->name, name) < 0)))
        p = p->next;

    CityPos newC = CreateCity(name, population);
    newC->next = p->next;
    p->next = newC;
}

void PrintCities(CityPos head) {
    CityPos p = head->next;
    while (p) {
        printf("   %s (%d)\n", p->name, p->population);
        p = p->next;
    }
}

void FreeCityList(CityPos head) {
    CityPos tmp;
    while (head->next) {
        tmp = head->next;
        head->next = tmp->next;
        free(tmp);
    }
}


CountryPos CreateCountry(char* name) {
    CountryPos c = malloc(sizeof(struct country));
    strcpy(c->name, name);
    c->left = c->right = NULL;
    c->head = malloc(sizeof(struct city));
    c->head->next = NULL;
    return c;
}

CountryPos InsertCountry(CountryPos root, char* name, char* file) {
    if (!root) {
        CountryPos c = CreateCountry(name);

        FILE* fp = fopen(file, "r");
        char city[20];
        int pop;

        while (fscanf(fp, " %[^,],%d", city, &pop) == 2)
            InsertCitySorted(c->head, city, pop);

        fclose(fp);
        return c;
    }

    if (strcmp(name, root->name) < 0)
        root->left = InsertCountry(root->left, name, file);
    else
        root->right = InsertCountry(root->right, name, file);

    return root;
}

void PrintCountries(CountryPos root) {
    if (!root) return;
    PrintCountries(root->left);
    printf("%s\n", root->name);
    PrintCities(root->head);
    PrintCountries(root->right);
}

void FreeCountries(CountryPos root) {
    if (!root) return;
    FreeCountries(root->left);
    FreeCountries(root->right);
    FreeCityList(root->head);
    free(root->head);
    free(root);
}


void SearchCities(CityPos head, int minPop) {
    CityPos p = head->next;
    while (p) {
        if (p->population > minPop)
            printf("   %s (%d)\n", p->name, p->population);
        p = p->next;
    }
}

void FindCountry(CountryPos root) {
    char name[20];
    int minPop;

    printf("Unesi drzavu: ");
    scanf("%s", name);
    printf("Unesi minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    while (root && strcmp(root->name, name))
        root = strcmp(name, root->name) < 0 ? root->left : root->right;

    if (root)
        SearchCities(root->head, minPop);
    else
        printf("Drzava ne postoji.\n");
}


int main() {
    FILE* fp = fopen("drzave.txt", "r");
    CountryPos root = NULL;
    char country[20], file[20];

    while (fscanf(fp, "%s %s", country, file) == 2)
        root = InsertCountry(root, country, file);

    fclose(fp);

    PrintCountries(root);
    FindCountry(root);
    FreeCountries(root);

    return 0;
}
