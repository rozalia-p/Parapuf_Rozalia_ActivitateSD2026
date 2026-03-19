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

void afisareCarte(Carte c) {
	printf("ID: %d\n", c.id);
	printf("Numar pagini: %d\n", c.nrPagini);
	printf("Autor: %s\n", c.autor);
	printf("Pret: %.2f\n", c.pret);
	printf("Categorie: %c\n\n", c.categorie);
}

void afisareVectorCarti(Carte* carti, int nrCarti) {
	if (carti != NULL) {
		for (int i = 0; i < nrCarti; i++) {
			afisareCarte(carti[i]);
		}
	}
}

void adaugaCarteInVector(Carte** carti, int* nrCarti, Carte carteNoua) {
	// Realocam spatiu pentru noul vector
	Carte* aux = (Carte*)malloc(sizeof(Carte) * ((*nrCarti) + 1));

	for (int i = 0; i < *nrCarti; i++) {
		aux[i] = (*carti)[i]; // Shallow copy pentru elementele existente
	}

	aux[(*nrCarti)] = carteNoua; // Adaugam noua carte

	if (*carti != NULL) {
		free(*carti); // Eliberam vechiul container (fara sa eliberam autorii, deoarece i-am copiat in aux)
	}

	(*carti) = aux;
	(*nrCarti)++;
}

Carte citireCarteFisier(FILE* file) {
	Carte c;
	char buffer[100];
	char sep[4] = ",;\n";

	if (fgets(buffer, 100, file)) {
		char* token = strtok(buffer, sep);
		if (token) c.id = atoi(token);

		token = strtok(NULL, sep);
		if (token) c.nrPagini = atoi(token);

		token = strtok(NULL, sep);
		if (token) {
			c.autor = (char*)malloc(strlen(token) + 1);
			strcpy(c.autor, token);
		}
		else {
			c.autor = NULL;
		}

		token = strtok(NULL, sep);
		if (token) c.pret = (float)atof(token);

		token = strtok(NULL, sep);
		if (token) c.categorie = token[0];
	}
	else {
		c.id = -1; // Flag in caz de citire esuata
		c.autor = NULL;
	}

	return c;
}

Carte* citireVectorCartiFisier(const char* numeFisier, int* nrCartiCitite) {
	FILE* file = fopen(numeFisier, "r");
	Carte* carti = NULL;
	(*nrCartiCitite) = 0;

	if (file) {
		while (!feof(file)) {
			Carte c = citireCarteFisier(file);
			if (c.id != -1) {
				adaugaCarteInVector(&carti, nrCartiCitite, c);
			}
		}
		fclose(file);
	}
	else {
		printf("Eroare la deschiderea fisierului!\n");
	}

	return carti;
}

void dezalocareVectorCarti(Carte** vector, int* nrCarti) {
	if (*vector != NULL) {
		for (int i = 0; i < *nrCarti; i++) {
			if ((*vector)[i].autor != NULL) {
				free((*vector)[i].autor);
			}
		}
		free(*vector);
		(*vector) = NULL;
		(*nrCarti) = 0;
	}
}

int main() {
	int nrCarti = 0;
	Carte* carti = citireVectorCartiFisier("carti.txt", &nrCarti);

	printf("--- Vectorul de Carti ---\n\n");
	afisareVectorCarti(carti, nrCarti);

	dezalocareVectorCarti(&carti, &nrCarti);

	return 0;
}