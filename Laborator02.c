#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

 

struct Carte initializare(int id, int nrPagini, const char* autor, float pret, char categorie) {
	struct Carte c;
	c.id = id;
	c.nrPagini = nrPagini;
	c.autor = (char*)malloc(sizeof(char) * (strlen(autor) + 1));
	strcpy(c.autor, autor);
	c.pret = pret;
	c.categorie = categorie;
	return c;
}

void afisare(struct Carte c) {
	if (c.autor != NULL) {
		printf("%d. Cartea scrisa de %s din categoria %c are %d pagini si costa %5.2f RON.\n",
			c.id, c.autor, c.categorie, c.nrPagini, c.pret);
	}
	else {
		printf("%d. Cartea din categoria %c are %d pagini si costa %5.2f RON.\n",
			c.id, c.categorie, c.nrPagini, c.pret);
	}
}

void afisareVector(struct Carte* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++) {
		afisare(vector[i]);
	}
}

struct Carte* copiazaPrimeleNElemente(struct Carte* vector, int nrElemente, int nrElementeCopiate) {
	struct Carte* vectorNou = NULL;
	vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * nrElementeCopiate);

	for (int i = 0; i < nrElementeCopiate; i++) {
		vectorNou[i] = vector[i];
		vectorNou[i].autor = (char*)malloc(strlen(vector[i].autor) + 1);
		strcpy(vectorNou[i].autor, vector[i].autor);
	}

	return vectorNou;
}

void dezalocare(struct Carte** vector, int* nrElemente) {
	for (int i = 0; i < (*nrElemente); i++) {
		if ((*vector)[i].autor != NULL) {
			free((*vector)[i].autor);
		}
	}
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaCartiScumpe(struct Carte* vector, int nrElemente, float pretMinim, struct Carte** vectorNou, int* dimensiune) {
	*dimensiune = 0;

	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*dimensiune)++;
		}
	}

	if ((*vectorNou) != NULL) {
		free(*vectorNou);
	}

	*vectorNou = (struct Carte*)malloc(sizeof(struct Carte) * (*dimensiune));

	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		if (vector[i].pret >= pretMinim) {
			(*vectorNou)[k] = vector[i];
			(*vectorNou)[k].autor = (char*)malloc(strlen(vector[i].autor) + 1);
			strcpy((*vectorNou)[k].autor, vector[i].autor);
			k++;
		}
	}
}

struct Carte getPrimaCarteByAutor(struct Carte* vector, int nrElemente, const char* autor) {
	struct Carte c;
	c.autor = NULL;

	for (int i = 0; i < nrElemente; i++) {
		if (strcmp(vector[i].autor, autor) == 0) {
			c = vector[i];
			c.autor = (char*)malloc(strlen(vector[i].autor) + 1);
			strcpy(c.autor, vector[i].autor);
			return c;
		}
	}

	c.id = -1;
	c.nrPagini = 0;
	c.pret = 0;
	c.categorie = '-';
	return c;
}

int main() {

	struct Carte* carti = NULL;
	int nrCarti = 3;

	carti = (struct Carte*)malloc(sizeof(struct Carte) * nrCarti);
	carti[0] = initializare(1, 350, "Eminescu", 45.5, 'L');
	carti[1] = initializare(2, 500, "Rebreanu", 60.0, 'R');
	carti[2] = initializare(3, 280, "Creanga", 30.0, 'B');

	printf("Vector initial de carti:\n");
	afisareVector(carti, nrCarti);

	struct Carte* primeleCarti = NULL;
	int nrPrimeleCarti = 2;
	primeleCarti = copiazaPrimeleNElemente(carti, nrCarti, nrPrimeleCarti);

	printf("\nPrimele carti:\n");
	afisareVector(primeleCarti, nrPrimeleCarti);

	dezalocare(&primeleCarti, &nrPrimeleCarti);

	printf("\nDupa dezalocarea vectorului primeleCarti:\n");
	afisareVector(primeleCarti, nrPrimeleCarti);

	struct Carte* cartiScumpe = NULL;
	int nrCartiScumpe = 0;
	copiazaCartiScumpe(carti, nrCarti, 40.0, &cartiScumpe, &nrCartiScumpe);

	printf("\nCarti scumpe:\n");
	afisareVector(cartiScumpe, nrCartiScumpe);

	dezalocare(&cartiScumpe, &nrCartiScumpe);

	struct Carte carte = getPrimaCarteByAutor(carti, nrCarti, "Rebreanu");
	printf("\nCarte gasita dupa autor:\n");
	afisare(carte);

	if (carte.autor != NULL) {
		free(carte.autor);
		carte.autor = NULL;
	}

	dezalocare(&carti, &nrCarti);

	return 0;
}