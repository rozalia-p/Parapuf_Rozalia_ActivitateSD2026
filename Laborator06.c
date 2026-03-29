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

struct HashTable {
	int dimensiune;
	Nod** vector;
};
typedef struct HashTable HashTable;


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dimensiune = dimensiune;
	ht.vector = (Nod**)malloc(sizeof(Nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++) {
		ht.vector[i] = NULL;
	}
	return ht;
}


int functieHash(int id, int dimensiune) {
	return id % dimensiune;
}


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


void inserareHashTable(HashTable ht, Carte c) {
	if (ht.dimensiune > 0) {
		int pozitie = functieHash(c.id, ht.dimensiune);
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = c;
		nou->next = ht.vector[pozitie];
		ht.vector[pozitie] = nou;
	}
}

void afisareCarte(Carte c) {
	printf("  [ID: %d] %s - %d pagini - %.2f RON (Cat: %c)\n",
		c.id, c.autor, c.nrPagini, c.pret, c.categorie);
}

void afisareHashTable(HashTable ht) {
	for (int i = 0; i < ht.dimensiune; i++) {
		printf("Slot [%d]:\n", i);
		Nod* temp = ht.vector[i];
		if (!temp) printf("  (gol)\n");
		while (temp) {
			afisareCarte(temp->info);
			temp = temp->next;
		}
	}
}


Carte cautareCarte(HashTable ht, int idCautat) {
	int pozitie = functieHash(idCautat, ht.dimensiune);
	Nod* temp = ht.vector[pozitie];
	while (temp) {
		if (temp->info.id == idCautat) {
			return temp->info;
		}
		temp = temp->next;
	}
	Carte eroare; eroare.id = -1; eroare.autor = NULL;
	return eroare;
}


void dezalocareHashTable(HashTable* ht) {
	for (int i = 0; i < ht->dimensiune; i++) {
		Nod* temp = ht->vector[i];
		while (temp) {
			Nod* aux = temp;
			temp = temp->next;
			free(aux->info.autor);
			free(aux);
		}
	}
	free(ht->vector);
	ht->vector = NULL;
	ht->dimensiune = 0;
}

int main() {
	
	HashTable ht = initializareHashTable(3);

	FILE* f = fopen("carti.txt", "r");
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteFisier(f);
			if (c.id != -1) {
				inserareHashTable(ht, c);
			}
		}
		fclose(f);
	}

	printf("--- CONTINUT HASH TABLE (CHAINING) ---\n");
	afisareHashTable(ht);

	int idDeCautat = 4;
	printf("\nCautam cartea cu ID %d:\n", idDeCautat);
	Carte gasita = cautareCarte(ht, idDeCautat);
	if (gasita.id != -1) {
		afisareCarte(gasita);
	}
	else {
		printf("Cartea nu a fost gasita.\n");
	}

	dezalocareHashTable(&ht);
	return 0;
}