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
	struct Nod* st;
	struct Nod* dr;
};
typedef struct Nod Nod;


int maxim_val(int a, int b) {
	return (a > b) ? a : b;
}

Carte citireCarteDinFisier(FILE* file) {
	char buffer[100];
	char sep[] = ",\n";
	Carte c1;
	c1.id = -1; 

	if (fgets(buffer, 100, file)) {
		char* aux;
		aux = strtok(buffer, sep);
		if (aux) c1.id = atoi(aux);

		aux = strtok(NULL, sep);
		if (aux) c1.nrPagini = atoi(aux);

		aux = strtok(NULL, sep);
		if (aux) {
			c1.autor = (char*)malloc(strlen(aux) + 1);
			strcpy(c1.autor, aux);
		}

		aux = strtok(NULL, sep);
		if (aux) c1.pret = (float)atof(aux);

		aux = strtok(NULL, sep);
		if (aux) c1.categorie = aux[0];
	}
	return c1;
}

void afisareCarte(Carte carte) {
	printf("Id: %d | Pagini: %d | Pret: %.2f | Categorie: %c | Autor: %s\n",
		carte.id, carte.nrPagini, carte.pret, carte.categorie, carte.autor);
}


int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina) {
		return 1 + maxim_val(calculeazaInaltimeArbore(radacina->st), calculeazaInaltimeArbore(radacina->dr));
	}
	return 0;
}


int gradEchilibru(Nod* radacina) {
	if (radacina) {
		return calculeazaInaltimeArbore(radacina->st) - calculeazaInaltimeArbore(radacina->dr);
	}
	return 0;
}


Nod* rotireDreapta(Nod* radacina) {
	Nod* nodSt = radacina->st;
	radacina->st = nodSt->dr;
	nodSt->dr = radacina;
	return nodSt;
}

Nod* rotireStanga(Nod* radacina) {
	Nod* nodDr = radacina->dr;
	radacina->dr = nodDr->st;
	nodDr->st = radacina;
	return nodDr;
}


Nod* adaugaCarteInArboreEchilibrat(Nod* radacina, Carte carteNoua) {
	if (radacina) {
		if (carteNoua.id < radacina->info.id) {
			radacina->st = adaugaCarteInArboreEchilibrat(radacina->st, carteNoua);
		}
		else if (carteNoua.id > radacina->info.id) {
			radacina->dr = adaugaCarteInArboreEchilibrat(radacina->dr, carteNoua);
		}
		else {
			return radacina; 
		}

		
		int grad = gradEchilibru(radacina);

		if (grad == 2 && gradEchilibru(radacina->st) == 1) {
			return rotireDreapta(radacina);
		}
		if (grad == -2 && gradEchilibru(radacina->dr) == -1) {
			return rotireStanga(radacina);
		}
		if (grad == 2 && gradEchilibru(radacina->st) == -1) {
			radacina->st = rotireStanga(radacina->st);
			return rotireDreapta(radacina);
		}
		if (grad == -2 && gradEchilibru(radacina->dr) == 1) {
			radacina->dr = rotireDreapta(radacina->dr);
			return rotireStanga(radacina);
		}

		return radacina;
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = carteNoua;
		nou->st = NULL;
		nou->dr = NULL;
		return nou;
	}
}


Nod* citireArboreDeCartiDinFisier(const char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	if (f) {
		while (!feof(f)) {
			Carte c = citireCarteDinFisier(f);
			if (c.id != -1) {
				radacina = adaugaCarteInArboreEchilibrat(radacina, c);
			}
		}
		fclose(f); 
	}
	return radacina;
}


void afisarePreordine(Nod* radacina) { 
	if (radacina) {
		afisareCarte(radacina->info);
		afisarePreordine(radacina->st);
		afisarePreordine(radacina->dr);
	}
}

void afisareInordine(Nod* radacina) { 
	if (radacina) {
		afisareInordine(radacina->st);
		afisareCarte(radacina->info);
		afisareInordine(radacina->dr);
	}
}

void afisarePostordine(Nod* radacina) { 
	if (radacina) {
		afisarePostordine(radacina->st);
		afisarePostordine(radacina->dr);
		afisareCarte(radacina->info);
	}
}


void dezalocareArboreDeCarti(Nod** radacina) {
	if (*radacina) {
		dezalocareArboreDeCarti(&((*radacina)->st));
		dezalocareArboreDeCarti(&((*radacina)->dr));
		free((*radacina)->info.autor);
		free(*radacina);
		*radacina = NULL;
	}
}


Carte getCarteByID(Nod* radacina, int id) {
	if (radacina) {
		if (radacina->info.id == id) {
			return radacina->info;
		}
		if (id < radacina->info.id) {
			return getCarteByID(radacina->st, id);
		}
		else {
			return getCarteByID(radacina->dr, id);
		}
	}
	Carte c; c.id = -1; c.autor = NULL;
	return c;
}


int determinaNumarNoduri(Nod* radacina) {
	if (radacina) {
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);
	}
	return 0;
}

float calculeazaPretTotal(Nod* radacina) {
	if (radacina) {
		return radacina->info.pret + calculeazaPretTotal(radacina->st) + calculeazaPretTotal(radacina->dr);
	}
	return 0;
}

float calculeazaPretulCartilorUnuiAutor(Nod* radacina, const char* autor) {
	if (radacina) {
		float pretCurent = 0;
		if (strcmp(radacina->info.autor, autor) == 0) {
			pretCurent = radacina->info.pret;
		}
		return pretCurent + calculeazaPretulCartilorUnuiAutor(radacina->st, autor) + calculeazaPretulCartilorUnuiAutor(radacina->dr, autor);
	}
	return 0;
}

int main() {

	Nod* radacina = citireArboreDeCartiDinFisier("carti.txt");

	printf("--- Parcurgere INORDINE (Sortat dupa ID) ---\n");
	afisareInordine(radacina);

	printf("\n--- Parcurgere PREORDINE ---\n");
	afisarePreordine(radacina);

	printf("\nInaltime arbore: %d\n", calculeazaInaltimeArbore(radacina));
	printf("Numar total de carti (noduri): %d\n", determinaNumarNoduri(radacina));
	printf("Valoarea totala a bibliotecii: %.2f RON\n", calculeazaPretTotal(radacina));

	const char* autorCautat = "George Orwell";
	printf("Valoarea cartilor scrise de %s: %.2f RON\n", autorCautat, calculeazaPretulCartilorUnuiAutor(radacina, autorCautat));

	printf("\nCautare carte cu ID 3:\n");
	Carte gasita = getCarteByID(radacina, 3);
	if (gasita.id != -1) {
		afisareCarte(gasita);
	}
	else {
		printf("Cartea cu ID-ul cerut nu a fost gasita.\n");
	}

	dezalocareArboreDeCarti(&radacina);
	return 0;
}