//#include<stdio.h>
//#include<stdlib.h>
//#include<string.h>
//
//struct Masina {
//	int id;
//	int putere;
//	char* marca;
//	float pret;
//	char clasa;
//};
//
//struct Masina initializare(int id, int putere, const char* marca, float pret, char clasa) {
//	struct Masina m;
//	m.id = id;
//	m.putere = putere;
//	m.marca = (char*)malloc(sizeof(char) * (strlen(marca) + 1));
//	strcpy(m.marca, marca);
//	m.pret = pret;
//	m.clasa = clasa;
//	return m;
//}
//
//void afisare(struct Masina m) {
//	if (m.marca != NULL) {
//		printf("%d. Masina marca %s din clasa %c are %d CP si costa %5.2f euro.\n",
//			m.id, m.marca, m.clasa, m.putere, m.pret);
//	}
//	else {
//		printf("%d. Masina din clasa %c are %d CP si costa %5.2f euro.\n",
//			m.id, m.clasa, m.putere, m.pret);
//	}
//}
//
//void modificaPret(struct Masina* m, float noulPret) {
//	if (noulPret > 0) {
//		m->pret = noulPret;
//	}
//}
//
//void dezalocare(struct Masina* m) {
//	if (m->marca != NULL) {
//		free(m->marca);
//		m->marca = NULL;
//	}
//}
//
//int main() {
//	struct Masina m;
//	m = initializare(1, 150, "BMW", 25000.5, 'C');
//
//	afisare(m);
//
//	modificaPret(&m, 22000);
//	afisare(m);
//
//	dezalocare(&m);
//	afisare(m);
//
//	return 0;
//}