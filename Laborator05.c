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
	struct Nod* prev; 
};
typedef struct Nod Nod;


struct LDI {
	Nod* prim;
	Nod* ultim;
};
typedef struct LDI LDI;


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


void inserareSfarsit(LDI* lista, Carte c) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = c;
	nou->next = NULL;
	nou->prev = lista->ultim;

	if (lista->ultim != NULL) {
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
	else {
		
		lista->prim = nou;
		lista->ultim = nou;
	}
}

void afisareCarte(Carte c) {
	printf("ID: %d | Pagini: %d | Autor: %s | Pret: %.2f | Cat: %c\n",
		c.id, c.nrPagini, c.autor, c.pret, c.categorie);
}

void afisareDeLaInceput(LDI lista) {
	Nod* temp = lista.prim;
	while (temp) {
		afisareCarte(temp->info);
		temp = temp->next;
	}
}

void afisareDeLaSfarsit(LDI lista) {
	Nod* temp = lista.ultim;
	while (temp) {
		afisareCarte(temp->info);
		temp = temp->prev;
	}
}

void dezalocareLDI(LDI* lista) {
	Nod* temp = lista->prim;
	while (temp) {
		Nod* aux = temp;
		temp = temp->next;
		free(aux->info.autor);
		free(aux);
	}
	lista->prim = NULL;
	lista->ultim = NULL;
}


void stergeCarteDupaID(LDI* lista, int idCautat) {
	Nod* temp = lista->prim;
	while (temp) {
		if (temp->info.id == idCautat) {
			
			if (temp->prev) {
				temp->prev->next = temp->next;
			}
			else {
				lista->prim = temp->next;
			}

			
			if (temp->next) {
				temp->next->prev = temp->prev;
			}
			else {
				lista->ultim = temp->prev;
			}

			free(temp->info.autor);
			free(temp);
			return; 
		}
		temp = temp->next;
	}
}

int main() {
	LDI biblioteca;
	biblioteca.prim = NULL;
	biblioteca.ultim = NULL;

	FILE* f = fopen("carti.txt", "r");
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteFisier(f);
			if (c.id != -1) {
				inserareSfarsit(&biblioteca, c);
			}
		}
		fclose(f);
	}

	printf("--- Afisare Normala (Inceput -> Sfarsit) ---\n");
	afisareDeLaInceput(biblioteca);

	printf("\n--- Afisare Inversa (Sfarsit -> Inceput) ---\n");
	afisareDeLaSfarsit(biblioteca);

	printf("\nStergem cartea cu ID 3...\n");
	stergeCarteDupaID(&biblioteca, 3);

	printf("\n--- Lista dupa stergere ---\n");
	afisareDeLaInceput(biblioteca);

	dezalocareLDI(&biblioteca);
	return 0;
}