#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikl {
    char naziv[100];
    int kolicina;
    float cijena;
    struct Artikl *next;
} Artikl;

typedef struct Racun {
    char datum[11];
    Artikl *artikli;
    struct Racun *next;
} Racun;

Racun* ucitajRacune();
Racun* umetniRacunSortirano(Racun *lista, Racun *novi);
Artikl* umetniArtiklSortirano(Artikl *lista, Artikl *novi);
void obradiDatotekuRacuna(Racun *racun, const char *filename);
int datumUOpsegu(const char *d, const char *od, const char *do_);
void upit(Racun *lista);
void obrisiSve(Racun *lista);

int main() {


    Racun *lista = ucitajRacune();
    if (!lista) {
        printf("Nema učitanih računa!\n");
        return 0;
    }

    upit(lista);

    obrisiSve(lista);
    return 0;
}



Racun* ucitajRacune() {
    FILE *f = fopen("racuni.txt", "r");
    if (!f) return NULL;

    Racun *lista = NULL;
    char filename[100];

    while (fscanf(f, "%s", filename) == 1) {
        FILE *rf = fopen(filename, "r");
        if (!rf) continue;

        Racun *novi = malloc(sizeof(Racun));
        novi->artikli = NULL;
        novi->next = NULL;

        fscanf(rf, "%s", novi->datum);
        obradiDatotekuRacuna(novi, filename);

        fclose(rf);

        lista = umetniRacunSortirano(lista, novi);
    }

    fclose(f);
    return lista;
}

Racun* umetniRacunSortirano(Racun *lista, Racun *novi) {
    if (!lista || strcmp(novi->datum, lista->datum) < 0) {
        novi->next = lista;
        return novi;
    }
    Racun *p = lista;
    while (p->next && strcmp(novi->datum, p->next->datum) > 0)
        p = p->next;
    novi->next = p->next;
    p->next = novi;
    return lista;
}

Artikl* umetniArtiklSortirano(Artikl *lista, Artikl *novi) {
    if (!lista || strcmp(novi->naziv, lista->naziv) < 0) {
        novi->next = lista;
        return novi;
    }
    Artikl *p = lista;
    while (p->next && strcmp(novi->naziv, p->next->naziv) > 0)
        p = p->next;
    novi->next = p->next;
    p->next = novi;
    return lista;
}

void obradiDatotekuRacuna(Racun *racun, const char *filename) {
    FILE *f = fopen(filename, "r");
    char temp[200];

    fgets(temp, sizeof(temp), f); // preskoči datum (već učitan)

    while (fgets(temp, sizeof(temp), f)) {
        Artikl *a = malloc(sizeof(Artikl));
        a->next = NULL;

        char naziv[100];
        int kol;
        float cij;

        sscanf(temp, "%[^,],%d,%f", naziv, &kol, &cij);
        strcpy(a->naziv, naziv);
        a->kolicina = kol;
        a->cijena = cij;

        racun->artikli = umetniArtiklSortirano(racun->artikli, a);
    }

    fclose(f);
}

int datumUOpsegu(const char *d, const char *od, const char *do_) {
    return strcmp(d, od) >= 0 && strcmp(d, do_) <= 0;
}

void upit(Racun *lista) {
    char artikl[100], od[11], do_[11];

    printf("Unesite naziv artikla: ");
    scanf("%s", artikl);
    printf("Unesite početni datum (YYYY-MM-DD): ");
    scanf("%s", od);
    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", do_);

    float ukupno = 0;
    int kolicina = 0;

    for (Racun *r = lista; r; r = r->next) {
        if (!datumUOpsegu(r->datum, od, do_)) continue;

        for (Artikl *a = r->artikli; a; a = a->next) {
            if (strcmp(a->naziv, artikl) == 0) {
                ukupno += a->cijena * a->kolicina;
                kolicina += a->kolicina;
            }
        }
    }

    printf("\n--- REZULTAT ---\n");
    printf("Artikl: %s\n", artikl);
    printf("Ukupno potrošeno: %.2f EUR\n", ukupno);
    printf("Ukupno kupljeno: %d kom\n", kolicina);
}

void obrisiSve(Racun *lista) {
    while (lista) {
        Racun *r = lista;
        lista = lista->next;

        Artikl *a = r->artikli;
        while (a) {
            Artikl *tmp = a;
            a = a->next;
            free(tmp);
        }

        free(r);
    }
}
