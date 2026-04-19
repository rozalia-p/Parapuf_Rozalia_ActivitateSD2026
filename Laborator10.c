#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct Carte {
	int id;
	int nrPagini;
	float pret;
	char* autor;
	char categorie;
};
typedef struct Carte Carte;

struct Nod {
	Carte info;
	struct Nod* next;
};
typedef struct Nod Nod;


struct Coada {
	Nod* fata;
	Nod* spate;
};
typedef struct Coada Coada;


Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[] = ",\n";
	Carte c;
	c.id = -1;

	if (fgets(buffer, 100, file)) {
		char* aux = strtok(buffer, sep);
		if (aux) c.id = atoi(aux);

		aux = strtok(NULL, sep);
		if (aux) c.nrPagini = atoi(aux);

		aux = strtok(NULL, sep);
		if (aux) {
			c.autor = (char*)malloc(strlen(aux) + 1);
			strcpy(c.autor, aux);
		}

		aux = strtok(NULL, sep);
		if (aux) c.pret = (float)atof(aux);

		aux = strtok(NULL, sep);
		if (aux) c.categorie = aux[0];
	}
	return c;
}

void afisareCarte(Carte c) {
	printf("ID: %d | Pagini: %d | Pret: %.2f | Categorie: %c | Autor: %s\n",
		c.id, c.nrPagini, c.pret, c.categorie, c.autor);
}



void pushStack(Nod** varf, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = *varf;
	*varf = nou;
}

int emptyStack(Nod* varf) {
	return varf == NULL;
}

Carte popStack(Nod** varf) {
	if (!emptyStack(*varf)) {
		Nod* temp = *varf;
		Carte extrasa = temp->info;
		*varf = (*varf)->next;
		free(temp); 
		return extrasa;
	}
	Carte eroare; eroare.id = -1; eroare.autor = NULL;
	return eroare;
}

Nod* citireStackCartiDinFisier(const char* numeFisier) {
	Nod* varf = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteDinFisier(f);
			if (c.id != -1) {
				pushStack(&varf, c);
			}
		}
		fclose(f);
	}
	return varf;
}

void dezalocareStivaDeCarti(Nod** varf) {
	while (!emptyStack(*varf)) {
		Carte extrasa = popStack(varf);
		free(extrasa.autor);
	}
}

int sizeStack(Nod* varf) {
	int contor = 0;
	while (varf) {
		contor++;
		varf = varf->next;
	}
	return contor;
}


void enqueue(Coada* c, Carte carteNoua) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = carteNoua;
	nou->next = NULL;

	if (c->spate == NULL) { 
		c->fata = nou;
		c->spate = nou;
	}
	else {
		c->spate->next = nou;
		c->spate = nou;
	}
}

int emptyQueue(Coada c) {
	return c.fata == NULL;
}

Carte dequeue(Coada* c) {
	if (!emptyQueue(*c)) {
		Nod* temp = c->fata;
		Carte extrasa = temp->info;
		c->fata = temp->next;

		if (c->fata == NULL) { 
			c->spate = NULL;
		}
		free(temp);
		return extrasa;
	}
	Carte eroare; eroare.id = -1; eroare.autor = NULL;
	return eroare;
}

Coada citireCoadaDeCartiDinFisier(const char* numeFisier) {
	Coada c;
	c.fata = NULL;
	c.spate = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Carte carte = citireCarteDinFisier(f);
			if (carte.id != -1) {
				enqueue(&c, carte);
			}
		}
		fclose(f);
	}
	return c;
}

void dezalocareCoadaDeCarti(Coada* c) {
	while (!emptyQueue(*c)) {
		Carte extrasa = dequeue(c);
		free(extrasa.autor);
	}
}


Carte getCarteByID(Nod** varfStiva, int idCautat) {
	Nod* stivaAuxiliara = NULL;
	Carte gasita; gasita.id = -1; gasita.autor = NULL;

	
	while (!emptyStack(*varfStiva)) {
		Carte extrasa = popStack(varfStiva);
		if (extrasa.id == idCautat && gasita.id == -1) {
			gasita = extrasa; 
		}
		pushStack(&stivaAuxiliara, extrasa); 
	}

	
	while (!emptyStack(stivaAuxiliara)) {
		pushStack(varfStiva, popStack(&stivaAuxiliara));
	}

	return gasita;
}


float calculeazaPretTotalStiva(Nod** varfStiva) {
	Nod* stivaAuxiliara = NULL;
	float sumaTotala = 0;

	
	while (!emptyStack(*varfStiva)) {
		Carte extrasa = popStack(varfStiva);
		sumaTotala += extrasa.pret;
		pushStack(&stivaAuxiliara, extrasa);
	}

	
	while (!emptyStack(stivaAuxiliara)) {
		pushStack(varfStiva, popStack(&stivaAuxiliara));
	}

	return sumaTotala;
}

int main() {

	printf("--- STIVA (LIFO) ---\n");
	Nod* stivaCarti = citireStackCartiDinFisier("carti.txt");

	printf("Avem %d carti in stiva.\n", sizeStack(stivaCarti));
	printf("Valoarea totala a stivei este: %.2f RON\n", calculeazaPretTotalStiva(&stivaCarti));

	int idDeCautat = 3;
	printf("\nCautam cartea cu ID %d in stiva:\n", idDeCautat);
	Carte c = getCarteByID(&stivaCarti, idDeCautat);
	if (c.id != -1) {
		afisareCarte(c);
	}
	else {
		printf("Cartea cu ID %d nu a fost gasita!\n", idDeCautat);
	}

	printf("\nExtragem (Pop) cartile ramase in stiva (Ies in ordine inversa fata de cum au intrat):\n");
	while (!emptyStack(stivaCarti)) {
		Carte extrasa = popStack(&stivaCarti);
		afisareCarte(extrasa);
		free(extrasa.autor); 
	}

	printf("\n-----------------------------------\n\n");

	
	printf("--- COADA (FIFO) ---\n");
	Coada coadaCarti = citireCoadaDeCartiDinFisier("carti.txt");

	printf("Extragem (Dequeue) cartile din coada (Ies in exact aceeasi ordine in care au fost in fisier):\n");
	while (!emptyQueue(coadaCarti)) {
		Carte extrasa = dequeue(&coadaCarti);
		afisareCarte(extrasa);
		free(extrasa.autor);
	}

	return 0;
}