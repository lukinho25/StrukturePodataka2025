#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX 100


//f-ja1 - fopen u read, EOL ili "\n" i brojač++, fclose datoteku

//buffer[0]

//f-ja2 (tip*)malloc(n*sizeof(tip)), fopen(dat, r), buffer = fgets..., fclose dat

/*typedef struct student {
	varijable;
	}Stud; */
	
//f-ja3 UI, printf: Ime, Prezime, Bodovi, Relativan broj bodova

//f-ja 4: relativan broj bodova


typedef struct student{

	char ime[50];
	char prezime[50];
	int bodovi;

} Stud;


int brojac(const char* dat) {

	FILE* fp = fopen(dat, "r");

	if (fp == NULL) {

		perror("Greška!");
		return -1;
	}

	int brojac = 0;
	char znak;

	while((znak = fgetc(fp)) != EOF)  {
		if(znak == '\n')
		brojac++;
	}

	fclose(fp);

	return brojac;

}

Stud *ucitaj(const char *dat, int brojStud) {
	
	Stud* niz = (Stud*)malloc(brojStud * sizeof(Stud));
	if (niz == NULL) {
		return NULL;
	}

	FILE* fp = fopen(dat, "r");
	if (fp == NULL) {
		free(niz);
		return NULL;
	}

	char buffer[MAX];
	int i = 0;
	while (i < brojStud && fscanf(fp, "%s %s %d", niz[i].ime, niz[i].prezime, &niz[i].bodovi) == 3) {
		i++;
	}



	fclose(fp);
	return niz;
}

float relBod(int bodovi, int maxBodovi) {
	return ((float)bodovi / maxBodovi) * 100;
}


void ispis(Stud* niz, int brojStud, int maxBodovi) {
	printf("Ime\tPrezime\tBodovi\tRelativno\n");
	for (int i = 0; i < brojStud; i++) {
		float rel = relBod(niz[i].bodovi, maxBodovi);
		printf("%s\t%s\t%d\t%.2f%%\n", niz[i].ime, niz[i].prezime, niz[i].bodovi, rel);
	}
}


int main() {

	const char* datoteka = "studenti.txt";
	int maxBodovi = 100;

	int broj = brojac(datoteka);
	if (broj == -1) {
		printf("Greška pri otvaranju datoteke.\n");
		return -1;
	}

	Stud* studenti = ucitaj(datoteka, broj);
	if (studenti == NULL) {
		printf("Greška pri učitavanju studenata.\n");
		return -2;
	}

	ispis(studenti, broj, maxBodovi);

	free(studenti);
	return 0;
}





