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
	printf("[ID: %d] Autor: %s | Pagini: %d | Pret: %.2f RON | Categorie: %c\n",
		c.id, c.autor, c.nrPagini, c.pret, c.categorie);
}


typedef struct NodSecundar NodSecundar;
typedef struct NodPrincipal NodPrincipal;

struct NodPrincipal {
	Carte info;
	NodSecundar* vecini;
	struct NodPrincipal* next;
};

struct NodSecundar {
	NodPrincipal* nodInfo;
	NodSecundar* next;
};


void inserareListaPrincipala(NodPrincipal** cap, Carte c) {
	NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
	nou->info = c;
	nou->next = NULL;
	nou->vecini = NULL;

	if (*cap) {
		NodPrincipal* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

void inserareListaSecundara(NodSecundar** cap, NodPrincipal* nodInfo) {
	NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
	nou->nodInfo = nodInfo;
	nou->next = *cap;
	*cap = nou; 
}

NodPrincipal* cautaNodDupaID(NodPrincipal* cap, int idCautat) {
	while (cap && cap->info.id != idCautat) {
		cap = cap->next;
	}
	return cap;
}


void inserareMuchie(NodPrincipal* graf, int idStart, int idStop) {
	NodPrincipal* nodStart = cautaNodDupaID(graf, idStart);
	NodPrincipal* nodStop = cautaNodDupaID(graf, idStop);

	if (nodStart && nodStop) {
		inserareListaSecundara(&(nodStart->vecini), nodStop);
		inserareListaSecundara(&(nodStop->vecini), nodStart);
	}
}


NodPrincipal* citireNoduriCartiDinFisier(const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	NodPrincipal* graf = NULL;
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteDinFisier(f);
			if (c.id != -1) {
				inserareListaPrincipala(&graf, c);
			}
		}
		fclose(f);
	}
	return graf;
}

void citireMuchiiDinFisier(NodPrincipal* graf, const char* numeFisier) {
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			int idStart = 0, idStop = 0;
			if (fscanf(f, "%d %d", &idStart, &idStop) == 2) {
				inserareMuchie(graf, idStart, idStop);
			}
		}
		fclose(f);
	}
}

void dezalocareListaSecundara(NodSecundar** cap) {
	while (*cap) {
		NodSecundar* temp = (*cap);
		(*cap) = (*cap)->next;
		free(temp);
	}
}

void dezalocareNoduriGraf(NodPrincipal** graf) {
	while (*graf) {
		NodPrincipal* temp = (*graf);
		(*graf) = (*graf)->next;

		dezalocareListaSecundara(&(temp->vecini));
		free(temp->info.autor); 
		free(temp);
	}
}


int calculeazaNrNoduriGraf(NodPrincipal* listaPrincipala) {
	int count = 0;
	while (listaPrincipala) {
		count++;
		listaPrincipala = listaPrincipala->next;
	}
	return count;
}


typedef struct NodStiva NodStiva;
struct NodStiva {
	int id;
	NodStiva* next;
};

void push(NodStiva** cap, int id) {
	NodStiva* nou = (NodStiva*)malloc(sizeof(NodStiva));
	nou->id = id;
	nou->next = *cap;
	*cap = nou;
}

int pop(NodStiva** cap) {
	if (*cap) {
		NodStiva* temp = *cap;
		*cap = (*cap)->next;
		int id = temp->id;
		free(temp);
		return id;
	}
	return -1;
}

void afisareGrafInAdancime(NodPrincipal* graf, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	unsigned char* vizitate = (unsigned char*)calloc(nrNoduri, sizeof(unsigned char));

	NodStiva* stiva = NULL;
	vizitate[idPlecare - 1] = 1;
	push(&stiva, idPlecare);

	while (stiva) {
		int idNod = pop(&stiva);
		NodPrincipal* nodCurent = cautaNodDupaID(graf, idNod);
		afisareCarte(nodCurent->info);

		NodSecundar* p = nodCurent->vecini;
		while (p) {
			
			if (vizitate[p->nodInfo->info.id - 1] == 0) {
				push(&stiva, p->nodInfo->info.id);
				vizitate[p->nodInfo->info.id - 1] = 1;
			}
			p = p->next;
		}
	}
	free(vizitate);
}


typedef struct NodCoada NodCoada;
struct NodCoada {
	int id;
	NodCoada* next;
};

void enqueue(NodCoada** cap, int id) {
	NodCoada* nou = (NodCoada*)malloc(sizeof(NodCoada));
	nou->id = id;
	nou->next = NULL;

	if (*cap) {
		NodCoada* p = *cap;
		while (p->next) {
			p = p->next;
		}
		p->next = nou;
	}
	else {
		*cap = nou;
	}
}

int dequeue(NodCoada** cap) {
	if (*cap) {
		NodCoada* temp = *cap;
		*cap = (*cap)->next;
		int id = temp->id;
		free(temp);
		return id;
	}
	return -1;
}

void afisareGrafInLatime(NodPrincipal* graf, int idPlecare) {
	int nrNoduri = calculeazaNrNoduriGraf(graf);
	unsigned char* vizitate = (unsigned char*)calloc(nrNoduri, sizeof(unsigned char));

	NodCoada* coada = NULL;
	vizitate[idPlecare - 1] = 1;
	enqueue(&coada, idPlecare);

	while (coada) {
		int idNod = dequeue(&coada);
		NodPrincipal* nodCurent = cautaNodDupaID(graf, idNod);
		afisareCarte(nodCurent->info);

		NodSecundar* p = nodCurent->vecini;
		while (p) {
			if (vizitate[p->nodInfo->info.id - 1] == 0) {
				enqueue(&coada, p->nodInfo->info.id);
				vizitate[p->nodInfo->info.id - 1] = 1;
			}
			p = p->next;
		}
	}
	free(vizitate);
}
int determinaNumarConexiuniCarte(NodPrincipal* graf, int idCarte) {

	NodPrincipal* nodGasit = cautaNodDupaID(graf, idCarte);

	if (nodGasit == NULL) {
		printf("Cartea cu ID %d nu exista in graf.\n", idCarte);
		return 0;
	}

	int numarLegaturi = 0;
	
	NodSecundar* p = nodGasit->vecini;

	
	while (p != NULL) {
		numarLegaturi++;
		p = p->next;
	}

	return numarLegaturi;
}


int main() {
	
	NodPrincipal* graf = citireNoduriCartiDinFisier("carti.txt");

	
	citireMuchiiDinFisier(graf, "muchii.txt");

	printf("=== Parcurgere in LATIME (BFS) incepand cu Cartea ID 1 ===\n");
	printf("-> Exploreaza mai intai cartile direct recomandate (vecinii directi).\n");
	afisareGrafInLatime(graf, 1);

	printf("\n=== Parcurgere in ADANCIME (DFS) incepand cu Cartea ID 1 ===\n");
	printf("-> Merge pe firul recomandarilor pana la capat inainte de a se intoarce.\n");
	afisareGrafInAdancime(graf, 1);

	int idCautat = 1;
	int nrConexiuni = determinaNumarConexiuniCarte(graf, idCautat);

	printf("\nCartea cu ID %d are %d recomandari asociate.\n", idCautat, nrConexiuni);
	
	dezalocareNoduriGraf(&graf);

	return 0;
}