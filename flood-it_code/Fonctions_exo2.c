#include<stdio.h>
#include "Entete_Fonctions.h"


void trouve_zone_imp(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){
	int couleur_Zsg = M[0][0];

	ListeCase *P = malloc(sizeof(ListeCase));
	if (P==NULL) exit(1);
	init_liste(P);
	Elnt_liste *cour=(*P);	
	ajoute_en_tete(P, i, j);

	while (!test_liste_vide(P)) { 		// Au debut, la pile P ne contient que (0,0)
		i=(*P)->i; j= (*P)->j; 		// On examine le dernier element ajouté à P
		if (!((i==0)&&(j==0))) {
			if (M[i][j] == couleur_Zsg) {		// Si la case examinée est de couleur M[0][0]
				ajoute_en_tete(L, i, j);	// On ajoute la case i,j à la liste L
				M[i][j] = -1 ;
				*taille = *taille + 1;
			}
		}
		
		// On collecte les cases adjacentes pas encore traitées et avec la meme couleur_Zsg
		if ( (i != 0) && (M[i-1][j] != -1) && (M[i-1][j] == couleur_Zsg) ) {
			enleve_en_tete(P, &i, &j);
			ajoute_en_tete(P, i-1, j);
			ajoute_en_tete(P, i, j);
		} 
		if ( (j != 0) && (M[i][j-1] != -1) && (M[i][j-1] == couleur_Zsg) ) {
			enleve_en_tete(P, &i, &j);
			ajoute_en_tete(P, i, j-1);
			ajoute_en_tete(P, i, j);
		}
		if ( (i < nbcase - 1 ) && (M[i+1][j] != -1) && (M[i+1][j] == couleur_Zsg) ) {
			enleve_en_tete(P, &i, &j);
			ajoute_en_tete(P, i+1, j);
			ajoute_en_tete(P, i, j);
		}
		if ( (j < nbcase - 1 ) && (M[i][j+1] != -1) && (M[i][j+1] == couleur_Zsg) ) {
			enleve_en_tete(P, &i, &j);
			ajoute_en_tete(P, i, j+1);
			ajoute_en_tete(P, i, j);	
		}
		
		// On elimine de la pile l'element qu'on vient de traiter
		enleve_en_tete(P, &i, &j);		
	}

	detruit_liste(P);
	free(P);
}


int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff){
  int cpt=0;
	int new_Zsg;
	int taille = 1;
	int tot_cases = dim * dim;

	while (taille != tot_cases) {
		new_Zsg = M[0][0];
		taille = 1;

		ListeCase *L = malloc(sizeof(ListeCase));
		if (L==NULL) exit(1);
		init_liste(L);
		trouve_zone_imp(M, dim, 0 , 0, &taille, L);

		// Nouvel tour de jeu : choix d'une nouvelle couleur, correspondant à un indice aleatoire 
		while ( new_Zsg == M[0][0]) {
			new_Zsg = rand()%(nbcl);	
		}
		cpt++;

		// On affecte la nouvelle couleur new_Zsg à toute case enregistrée dans la liste L	
		Elnt_liste *cour=(*L);
		while (cour!=NULL){
			M[cour->i][cour->j] = new_Zsg; // On attribue la nouvelle couleur choisie à chaque case enregistrée dans L
			if (aff == 1) Grille_attribue_couleur_case(G, cour->i, cour->j, M[cour->i][cour->j]);
    			cour=cour->suiv;
		}
		detruit_liste(L);
		free(L);

		M[0][0] = new_Zsg; // On attribue la nouvelle couleur choisie à M[0][0]
		if (aff == 1) Grille_attribue_couleur_case(G, 0, 0, M[0][0]);

		if (aff == 1)	Grille_redessine_Grille();
	}
  return cpt;
}


