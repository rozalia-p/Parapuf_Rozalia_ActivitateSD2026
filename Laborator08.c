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
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;


Nod* inserareABC(Nod* radacina, Carte c) {
	if (radacina != NULL) {
		if (c.id < radacina->info.id) {
			radacina->st = inserareABC(radacina->st, c);
		}
		else if (c.id > radacina->info.id) {
			radacina->dr = inserareABC(radacina->dr, c);
		}
		return radacina;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = c;
		nou->st = NULL;
		nou->dr = NULL;
		return nou;
	}
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

void afisareCarte(Carte c) {
	printf("ID: %d | Autor: %s | Pagini: %d | Pret: %.2f\n",
		c.id, c.autor, c.nrPagini, c.pret);
}


void afisareInordine(Nod* radacina) {
	if (radacina) {
		afisareInordine(radacina->st);
		afisareCarte(radacina->info);
		afisareInordine(radacina->dr);
	}
}


Nod* cautareCarte(Nod* radacina, int idCautat) {
	if (radacina == NULL || radacina->info.id == idCautat) {
		return radacina;
	}
	if (idCautat < radacina->info.id) {
		return cautareCarte(radacina->st, idCautat);
	}
	return cautareCarte(radacina->dr, idCautat);
}


void dezalocareABC(Nod** radacina) {
	if (*radacina) {
		dezalocareABC(&((*radacina)->st));
		dezalocareABC(&((*radacina)->dr));
		free((*radacina)->info.autor);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* radacina = NULL;
	FILE* f = fopen("carti.txt", "r");

	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteFisier(f);
			if (c.id != -1) {
				radacina = inserareABC(radacina, c);
			}
		}
		fclose(f);
	}

	printf("--- Biblioteca in format ABC (Sortata dupa ID) ---\n");
	afisareInordine(radacina);

	int idCautat = 4;
	printf("\nCautam cartea cu ID %d:\n", idCautat);
	Nod* rezultat = cautareCarte(radacina, idCautat);
	if (rezultat) {
		afisareCarte(rezultat->info);
	}
	else {
		printf("Cartea nu a fost gasita.\n");
	}

	dezalocareABC(&radacina);
	return 0;
}