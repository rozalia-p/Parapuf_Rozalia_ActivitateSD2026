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

struct Heap {
	Carte* vector;
	int dimensiune;
	int capacitate;
};
typedef struct Heap Heap;


Carte citireCarteFisier(FILE* f) {
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

void afisareCarte(Carte c) {
	printf("ID: %d | Pagini: %d | Autor: %s | Pret: %.2f\n",
		c.id, c.nrPagini, c.autor, c.pret);
}


void filtrareHeap(Heap h, int index) {
	int fiuSt = 2 * index + 1;
	int fiuDr = 2 * index + 2;
	int maxIdx = index;

	if (fiuSt < h.dimensiune && h.vector[fiuSt].nrPagini > h.vector[maxIdx].nrPagini) {
		maxIdx = fiuSt;
	}
	if (fiuDr < h.dimensiune && h.vector[fiuDr].nrPagini > h.vector[maxIdx].nrPagini) {
		maxIdx = fiuDr;
	}

	if (maxIdx != index) {
		Carte aux = h.vector[index];
		h.vector[index] = h.vector[maxIdx];
		h.vector[maxIdx] = aux;
		filtrareHeap(h, maxIdx);
	}
}


void inserareHeap(Heap* h, Carte c) {
	if (h->dimensiune < h->capacitate) {
		h->vector[h->dimensiune] = c;
		int curent = h->dimensiune;
		int parinte = (curent - 1) / 2;

		while (curent > 0 && h->vector[curent].nrPagini > h->vector[parinte].nrPagini) {
			Carte aux = h->vector[curent];
			h->vector[curent] = h->vector[parinte];
			h->vector[parinte] = aux;

			curent = parinte;
			parinte = (curent - 1) / 2;
		}
		h->dimensiune++;
	}
}


Carte extragereMax(Heap* h) {
	if (h->dimensiune > 0) {
		Carte rez = h->vector[0];
		h->vector[0] = h->vector[h->dimensiune - 1];
		h->dimensiune--;
		filtrareHeap(*h, 0);
		return rez;
	}
	Carte eroare; eroare.id = -1; return eroare;
}

void dezalocareHeap(Heap* h) {
	for (int i = 0; i < h->dimensiune; i++) {
		free(h->vector[i].autor);
	}
	free(h->vector);
	h->vector = NULL;
	h->dimensiune = 0;
}

int main() {
	Heap h;
	h.dimensiune = 0;
	h.capacitate = 10;
	h.vector = (Carte*)malloc(sizeof(Carte) * h.capacitate);

	FILE* f = fopen("carti.txt", "r");
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteFisier(f);
			if (c.id != -1) {
				inserareHeap(&h, c);
			}
		}
		fclose(f);
	}

	printf("--- Max-Heap Carti (dupa numarul de pagini) ---\n");
	for (int i = 0; i < h.dimensiune; i++) {
		afisareCarte(h.vector[i]);
	}

	printf("\nExtragere cartea cu cele mai multe pagini:\n");
	Carte maxPagini = extragereMax(&h);
	if (maxPagini.id != -1) {
		afisareCarte(maxPagini);
		free(maxPagini.autor); 
	}

	printf("\nHeap-ul dupa extragere:\n");
	for (int i = 0; i < h.dimensiune; i++) {
		afisareCarte(h.vector[i]);
	}

	dezalocareHeap(&h);
	return 0;
}