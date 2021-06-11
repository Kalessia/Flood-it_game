#include<stdlib.h>
#include<stdio.h>
#include "Entete_Fonctions.h"


/* Fonction qui alloue et construit une structure Graphe_zone a partir des informations contenues dans la matrice M.
Chaque sommet representera une zone de cases adjacentes dans M caracterisées par la meme couleur. */
Graphe_zone* cree_graphe_zone(int **M, int dim) {
	int k, w;

	Graphe_zone *J = malloc(sizeof(Graphe_zone));		// Allocation Graphe_zone
	if (J==NULL) exit(1);

	J->mat = (Sommet***) malloc(dim * sizeof(Sommet**)); 	// Allocation matrice de pointeurs sur sommet
	if (J->mat==NULL) exit(1);
	for (k=0; k<dim; k++) {	
		J->mat[k] = (Sommet**) malloc(dim * sizeof(Sommet*));	
		if (J->mat[k]==NULL) exit(1); }	

	initGraphe(J, dim);					// Initialisation Graphe_zone 

	for (k=0; k<dim; k++) {
		for (w=0; w<dim; w++) {
			if (J->mat[k][w] == NULL) {
				J->nbsom++;

				// Creation d'un nouveau sommet à partir des données dans M
				Sommet *s;
				s = (Sommet*) malloc(sizeof(Sommet));
				if (s==NULL) exit(1);
				initSommet(s, M[k][w], J->nbsom);
	
				// Creation d'une liste ListeCase partant du sommet s créé
				ListeCase *L = malloc(sizeof(ListeCase));
				if (L==NULL) exit(1);
				init_liste(L);	
	
				// On charge les cases de la meme zone du sommet s (J->mat[k][w]) dans la ListeCase cases 
				M[0][0] = s->cl; // cette instruction garantit la comptabilité avec la fonction trouve_zone_imp()
				trouve_zone_imp(M, dim, k, w, &(s->nbcase_som), L);
				if (k == 0 && w == 0) ajoute_en_tete(L, 0, 0);
				s->cases = (*L);

				// Enchainement du sommet s à la liste chainée de sommets du graphe (Cellule_som J->som)
				ajoute_liste_sommet(s, &J->som);
	
				// Mise à jour de la matrice mat : toute case correspondant a la zone trouve_zone_imp pointera vers ce s
				Elnt_liste *cour = s->cases;
				J->mat[k][w] = s;
				while (cour != NULL) {
					J->mat[cour->i][cour->j] = s;
					cour = cour->suiv;
				}
			}
		}
	}

	// On construit les aretes entre sommets/zones adjacent(e)s, en ajoutant pour chaque sommet s1 les sommets lui-adjacents
	// dans le graphe aux relatives listes 'sommet_adj'.
	// Pour trouver les sommets adjacents à s1 on explore la matrice mat à droite et en bas de la case mat[k][w] pointant s1.
	// NB. Notre graphe est non orienté, donc on lie les sommets adjacents s1 et s2 doublement :
	// sommet_adj de s1 contiendra s2, et sommet_adj de s2 contiendra s1. 
	for (k=0; k<dim; k++) {
		for (w=0; w<dim; w++) {
			if ( (k < dim-1) && (J->mat[k][w] != J->mat[k+1][w]) ) {
				if ( !sont_ils_adjacents(J->mat[k][w], J->mat[k+1][w]) ) {
					ajoute_voisin(J->mat[k][w], J->mat[k+1][w]);
					ajoute_voisin(J->mat[k+1][w], J->mat[k][w]);
				}
			}
			if ( (w < dim-1) && (J->mat[k][w] != J->mat[k][w+1]) ) {
				if ( !sont_ils_adjacents(J->mat[k][w], J->mat[k][w+1]) ) {
					ajoute_voisin(J->mat[k][w], J->mat[k][w+1]);
					ajoute_voisin(J->mat[k][w+1], J->mat[k][w]);
				}
			}
		}
	}

	return J;
}

/* Initialise une structure Graphe_zone */
void initGraphe(Graphe_zone *J, int dim){
	int k, w;
	J->nbsom = 0;
	J->dimM = dim;
	J->som = NULL;
	
	for (k=0; k<dim; k++) {
		for (w=0; w<dim; w++) {
			J->mat[k][w] = NULL; } } // Initialisation de G->mat à NULL
}

/* Initialise une structure Sommet */
void initSommet(Sommet *s, int clM, int incr) {
	s->num = incr;
	s->cl = clM;
	s->cases = NULL;
	s->nbcase_som = 0;
	s->sommet_adj = NULL;

	s->marque = 2;			// pour EXO5. Initialisation statut sommet : 2 = sommet non visite
	s->distance = INFINITO; 	// pour EXO6. La constante INFINITO est definie dans Graphe.h et vaut 2140000000
	s->pere = NULL; 		// pour EXO6. Pointeur vers le sommet predecesseur
}

/* Fonction qui ajoute une nouvelle Cellule_som contenant le sommet s à la liste passée en parametre.
Insertion à la fin pour garantir l'ordre des sommets dans l'affichage */
void ajoute_liste_sommet(Sommet *s, Cellule_som **ListeCellS) {

	Cellule_som *cellS;		// Creation d'une nouvelle Cellule_som
	cellS=(Cellule_som*) malloc(sizeof(Cellule_som));
	if (cellS==NULL) exit(1);

	cellS->sommet = s;
	cellS->suiv = NULL;
					
	if (*ListeCellS==NULL) {	// Si aucun sommet n'as pas encore ete insere alors on l'insere en tete
		*ListeCellS = cellS;
		return ;
	}

	Cellule_som *cour = *ListeCellS;
	while (cour->suiv != NULL) {
		cour = cour->suiv;
	}	
	cour->suiv = cellS;		// sinon on parcourt toute la liste chainee pour ajouter le sommet a la fin 
}

/* Detruit tous les elements d'une liste de Cellule_som passée en parametre et reinitialise la liste a NULL.
NB: la fonction ne detruit pas les sommets referenciés par les Cellule_som */
void detruit_liste_somme(Cellule_som **ListeCellS) {
	Cellule_som *cour = *ListeCellS;
	Cellule_som *tmp;
	while (cour != NULL) {
		tmp = cour;
		cour=cour->suiv;
		free(tmp);
	}
	(*ListeCellS) = NULL;
}

/* Creation d'une nouvelle Cellule_som representant une nouvelle arete entre s1 et s2.
La fonction met à jour s1 en ajoutant s2 a la liste sommet_adj de s1 */
void ajoute_voisin(Sommet *s1, Sommet *s2) {
	ajoute_liste_sommet(s2, &s1->sommet_adj);
}

/* La fonction retourne 1 si s1 et s2 sont adjacents, 0 sinon */
int sont_ils_adjacents(Sommet *s1, Sommet *s2) {
	Cellule_som *cour = s1->sommet_adj;
	while (cour != NULL) {
		if (cour->sommet == s2) return 1; 
		cour = cour->suiv;
	}
	return 0;
}

/* Affichage des sommets contenus dans une liste de Cellule_som */
void afficheListeCellule_som(Cellule_som **ListeCellS) { 
	Cellule_som *cour = (*ListeCellS);
	while (cour != NULL) {
		printf(" (%d) ", cour->sommet->num );
		cour = cour->suiv;
	}
	printf("\n");
}

/* Affichage d'un graphe_zone */
void afficheGraphe(Graphe_zone *J) {
	if (J->nbsom==0) { 
		printf("\nLe graphe ne contient aucun sommet\n\n");
		return;
	}

	printf("\nNombre de sommets du graphe J (representant le nombre de zones dans la grille) : %d\n\n", J->nbsom);
	Cellule_som *cour = J->som;
	while (cour != NULL) {
		printf(" [%d]	Cases  : ", cour->sommet->num );
		afficheListeCase(&(cour->sommet->cases));
		printf("     	Sommets adjacents : ");		
		afficheListeCellule_som(&(cour->sommet->sommet_adj));
		printf("\n");	
		printf("     	   | EXO5 Marque : %d\n", cour->sommet->marque);
		printf("\n");
		printf("     	   | EXO6 Distance depuis racine : %d\n", cour->sommet->distance);
		printf("     	   | EXO6 Sommet pere : %d\n", cour->sommet->pere);
		printf("\n\n");
		cour = cour->suiv;
	}
	printf("\n");
}

/* Insertion en tete d'une nouvelle Cellule_som à une liste de Cellule_som passée en parametre. */
void enfile(Sommet *s, Cellule_som **ListeCellS) {
	Cellule_som *cellS;
	cellS=(Cellule_som*) malloc(sizeof(Cellule_som));
	if (cellS==NULL) exit(1);

	cellS->sommet = s;
	cellS->suiv = *ListeCellS;
	*ListeCellS = cellS;
}

/* La fonction retourne le premier element en tete d'une liste de Cellule_som passée en parametre.
La Cellule_som contenant le sommet s est liberee. */
Sommet* defile(Cellule_som **ListeCellS) {
	Sommet *s = (*ListeCellS)->sommet;
	Cellule_som *tmp = *ListeCellS;
	*ListeCellS = (*ListeCellS)->suiv;
	free(tmp);
	return s;
}

/* Desalloue une structure Graphe_zone */
void libereGraphe(Graphe_zone *J) {
	int k, w;
	Cellule_som *cour = J->som;
	Cellule_som *tmp;
	while (cour!=NULL) {
		detruit_liste_somme(&(cour->sommet->sommet_adj));
		detruit_liste(&(cour->sommet->cases));
		free(cour->sommet); 	// cour->sommet = sommet pointé par l'element cour de la liste J->som
		J->nbsom--;
		cour = cour->suiv;
	}
	detruit_liste_somme(&(J->som));
	
	for(k=0; k < J->dimM; k++) {
		if (J->mat[k]) free(J->mat[k]);	} 

	free(J->mat);
	free(J);
}



