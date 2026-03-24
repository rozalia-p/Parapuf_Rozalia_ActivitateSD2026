#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Carte {
	int id;
	int nrPagini;
	char* autor;
	float pret;
	char categorie;
};
typedef struct Carte Carte;

struct Nod {
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;

void afisareCarte(Carte c) {
	printf("ID: %d, Pagini: %d, Autor: %s, Pret: %.2f, Categorie: %c\n",
		c.id, c.nrPagini, c.autor, c.pret, c.categorie);
}


void afisareLista(Nod* cap) {
	while (cap != NULL) {
		afisareCarte(cap->info);
		cap = cap->next;
	}
}


void inserareSfarsit(Nod** cap, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c; 
	nou->next = NULL;

	if (*cap != NULL) {
		Nod* temp = *cap;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		*cap = nou;
	}
}


Carte citireCarteDinFisier(FILE* f) {
	Carte c;
	char buffer[100];
	char sep[] = ",\n";

	if (fgets(buffer, 100, f)) {
		char* token = strtok(buffer, sep);
		c.id = atoi(token);

		token = strtok(NULL, sep);
		c.nrPagini = atoi(token);

		token = strtok(NULL, sep);
		c.autor = (char*)malloc(strlen(token) + 1);
		strcpy(c.autor, token);

		token = strtok(NULL, sep);
		c.pret = (float)atof(token);

		token = strtok(NULL, sep);
		c.categorie = token[0];
	}
	else {
		c.id = -1;
		c.autor = NULL;
	}
	return c;
}


Nod* citireListaDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f != NULL) {
		while (!feof(f)) {
			Carte c = citireCarteDinFisier(f);
			if (c.id != -1) {
				inserareSfarsit(&cap, c);
			}
		}
		fclose(f);
	}
	return cap;
}


void dezalocareLista(Nod** cap) {
	while (*cap != NULL) {
		Nod* temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.autor);
		free(temp);            
	}
}


float calculPretTotal(Nod* cap) {
	float suma = 0;
	while (cap != NULL) {
		suma += cap->info.pret;
		cap = cap->next;
	}
	return suma;
}

int main() {
	
	Nod* cap = citireListaDinFisier("carti.txt");

	printf("--- Lista de carti citita din fisier ---\n");
	afisareLista(cap);

	printf("\nValoarea totala a stocului de carti: %.2f\n", calculPretTotal(cap));

	dezalocareLista(&cap);

	if (cap == NULL) {
		printf("\nMemoria a fost eliberata cu succes.\n");
	}

	return 0;
}