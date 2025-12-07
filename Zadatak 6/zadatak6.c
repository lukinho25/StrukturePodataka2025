#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikl* PozicijaA;
typedef struct Racun* PozicijaR;

typedef struct Artikl {
    char naziv[100];
    int kolicina;
    float cijena;
    PozicijaA next;
} Artikl;

typedef struct Racun {
    char datum[11];
    PozicijaA artikli;
    PozicijaR next;
} Racun;

PozicijaR ucitajRacune(void);
PozicijaR umetniRacunSortirano(PozicijaR lista, PozicijaR novi);
PozicijaA umetniArtiklSortirano(PozicijaA lista, PozicijaA novi);
void obradiDatotekuRacuna(PozicijaR racun, const char *filename);
int datumUOpsegu(const char *d, const char *od, const char *do_);
void upit(PozicijaR lista);
void obrisiSve(PozicijaR lista);


int main(void) {

    PozicijaR lista = ucitajRacune();
    if (!lista) {
        printf("Nema učitanih računa!\n");
        return 0;
    }
    
    upit(lista);

    obrisiSve(lista);
    return 0;
}

PozicijaR ucitajRacune(void) { //funkcija koja učitava račune iz datoteke "racuni.txt"
    FILE *f = fopen("racuni.txt", "r");
    if (!f) return NULL;

    PozicijaR lista = NULL;
    char filename[100];

    while (fscanf(f, "%s", filename) == 1) {
        FILE *rf = fopen(filename, "r");
        if (!rf) continue;

        PozicijaR novi = malloc(sizeof(Racun));
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

PozicijaR umetniRacunSortirano(PozicijaR lista, PozicijaR novi) { //funkcija koja umetne racun u sortiranu listu racuna po datumu
    if (!lista || strcmp(novi->datum, lista->datum) < 0) {
        novi->next = lista;
        return novi;
    }

    PozicijaR p = lista;
    while (p->next && strcmp(novi->datum, p->next->datum) > 0)
        p = p->next;

    novi->next = p->next;
    p->next = novi;
    return lista;
}

PozicijaA umetniArtiklSortirano(PozicijaA lista, PozicijaA novi) { //funkcija koja umetne artikl u sortiranu listu artikala po nazivu
    if (!lista || strcmp(novi->naziv, lista->naziv) < 0) {
        novi->next = lista;
        return novi;
    }

    PozicijaA p = lista;
    while (p->next && strcmp(novi->naziv, p->next->naziv) > 0)
        p = p->next;

    novi->next = p->next;
    p->next = novi;
    return lista;
}

void obradiDatotekuRacuna(PozicijaR racun, const char *filename) { //funkcija koja obraduje datoteku pojedinog računa i popunjava listu artikala
    FILE *f = fopen(filename, "r");
    char temp[200];

    fgets(temp, sizeof(temp), f); // preskoci datum

    while (fgets(temp, sizeof(temp), f)) {
        PozicijaA a = malloc(sizeof(Artikl));
        a->next = NULL;

        char naziv[100];
        int kol;
        float cij;

        sscanf(temp, "%[^,],%d,%f", naziv, &kol, &cij);
        strcpy(a->naziv, naziv);
        a->kolicina = kol;
        a->cijena = cij;

        racun->artikli = umetniArtiklSortirano(racun->artikli, a); // umetni artikl u sortiranu listu artikala
    }

    fclose(f);
}

int datumUOpsegu(const char *d, const char *od, const char *do_) { //funkcija koja provjerava je li datum d u opsegu od do do_
    return strcmp(d, od) >= 0 && strcmp(d, do_) <= 0;
}

void upit(PozicijaR lista) { //funkcija koja obrađuje upit korisnika
    char artikl[100], od[11], do_[11];

    printf("Unesite naziv artikla: ");
    scanf("%s", artikl);
    printf("Unesite početni datum (YYYY-MM-DD): ");
    scanf("%s", od);
    printf("Unesite krajnji datum (YYYY-MM-DD): ");
    scanf("%s", do_);

    float ukupno = 0;
    int kolicina = 0;

    for (PozicijaR r = lista; r; r = r->next) { // prolazak kroz sve račune
        if (!datumUOpsegu(r->datum, od, do_)) continue; // preskoči račune izvan datuma

        for (PozicijaA a = r->artikli; a; a = a->next) { // prolazak kroz artikle na računu
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

void obrisiSve(PozicijaR lista) { //funkcija koja briše sve račune i artikle iz memorije
    while (lista) {
        PozicijaR r = lista;
        lista = lista->next;

        PozicijaA a = r->artikli;
        while (a) {
            PozicijaA tmp = a;
            a = a->next;
            free(tmp);
        }

        free(r);
    }
}
