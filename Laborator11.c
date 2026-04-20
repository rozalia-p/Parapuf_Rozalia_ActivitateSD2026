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


struct NodSecundar; 

struct NodPrincipal {
	Carte info;
	struct NodSecundar* vecini; 
	struct NodPrincipal* next;  
};
typedef struct NodPrincipal NodPrincipal;


struct NodSecundar {
	NodPrincipal* nodInfo;    
	struct NodSecundar* next;  
};
typedef struct NodSecundar NodSecundar;


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
	printf("[ID: %d] %s - %.2f RON (Cat: %c)\n",
		c.id, c.autor, c.pret, c.categorie);
}


void inserarePrincipala(NodPrincipal** graf, Carte carteNoua) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = carteNoua;
	nou->vecini = NULL;
	nou->next = NULL;

	if (*graf == NULL) {
		*graf = nou;
	}
	else {
		NodPrincipal* temp = *graf;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}

void inserareSecundara(NodSecundar** capSecundar, NodPrincipal* nodVecin) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->nodInfo = nodVecin;
	nou->next = NULL;

	if (*capSecundar == NULL) {
		*capSecundar = nou;
	}
	else {
		NodSecundar* temp = *capSecundar;
		while (temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = nou;
	}
}


NodPrincipal* cautaNodDupaID(NodPrincipal* graf, int id) {
	while (graf != NULL) {
		if (graf->info.id == id) {
			return graf;
		}
		graf = graf->next;
	}
	return NULL;
}


void inserareMuchie(NodPrincipal* graf, int idStart, int idStop) {
	NodPrincipal* nodStart = cautaNodDupaID(graf, idStart);
	NodPrincipal* nodStop = cautaNodDupaID(graf, idStop);

	
	if (nodStart != NULL && nodStop != NULL) {
		
		inserareSecundara(&(nodStart->vecini), nodStop);
		inserareSecundara(&(nodStop->vecini), nodStart);
	}
	else {
		printf("Eroare: Nu am putut crea legatura. Cartea %d sau %d nu exista!\n", idStart, idStop);
	}
}

NodPrincipal* citireNoduriCartiDinFisier(const char* numeFisier) {
	NodPrincipal* graf = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteDinFisier(f);
			if (c.id != -1) {
				inserarePrincipala(&graf, c);
			}
		}
		fclose(f);
	}
	else {
		printf("Fisierul de carti nu a fost gasit!\n");
	}
	return graf;
}

void citireMuchiiDinFisier(NodPrincipal* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		char buffer[50];
		while (fgets(buffer, 50, f)) {
			int idStart = atoi(strtok(buffer, ","));
			int idStop = atoi(strtok(NULL, ",\n"));
			inserareMuchie(graf, idStart, idStop);
		}
		fclose(f);
	}
	else {
		printf("Fisierul de legaturi (muchii) nu a fost gasit!\n");
	}
}


void afisareGraf(NodPrincipal* graf) {
	NodPrincipal* tempGraf = graf;
	while (tempGraf != NULL) {
		printf("\nCartea principala: ");
		afisareCarte(tempGraf->info);

		printf(" -> Carti recomandate/conexe:\n");
		NodSecundar* vecini = tempGraf->vecini;
		if (vecini == NULL) {
			printf("      (niciuna momentan)\n");
		}
		while (vecini != NULL) {
			printf("      * ");
			afisareCarte(vecini->nodInfo->info);
			vecini = vecini->next;
		}
		tempGraf = tempGraf->next;
	}
}


void dezalocareNoduriGraf(NodPrincipal** graf) {
	while (*graf != NULL) {
		NodPrincipal* nodDeSters = *graf;
		*graf = (*graf)->next;

		
		NodSecundar* tempVecin = nodDeSters->vecini;
		while (tempVecin != NULL) {
			NodSecundar* vecinDeSters = tempVecin;
			tempVecin = tempVecin->next;
			free(vecinDeSters);
		}

		
		free(nodDeSters->info.autor);

		
		free(nodDeSters);
	}
}

int main() {
	

	
	NodPrincipal* graf = citireNoduriCartiDinFisier("carti.txt");

	citireMuchiiDinFisier(graf, "muchii.txt");

	
	printf("=== RETEA DE CARTI (GRAF) ===\n");
	afisareGraf(graf);

	dezalocareNoduriGraf(&graf);

	return 0;
}