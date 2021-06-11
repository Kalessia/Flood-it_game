#include<stdio.h>
#include "Entete_Fonctions.h"


void trouve_zone_rec(int **M, int nbcase, int i, int j, int *taille, ListeCase *L){
	int couleur_Zsg = M[0][0];
	
	if (!((i==0)&&(j==0))) { // On ne modifie pas M[0][0] pour garder la couleur de reference
		if (M[i][j] == couleur_Zsg) {		// Si la case examinée est de couleur M[0][0]
			ajoute_en_tete(L, i, j);	// On ajoute la case i,j à la liste L
			M[i][j] = -1 ;
			*taille = *taille + 1;
		} else {
			return;
		}
	}
	// On collecte les cases adjacentes à la Zsg pas encore traitées
	if ( (i != 0) && (M[i-1][j] != -1) ) trouve_zone_rec(M, nbcase, i-1 , j, taille, L);
	if ( (j != 0) && (M[i][j-1] != -1) ) trouve_zone_rec(M, nbcase, i , j-1, taille, L);	
	if ( (i < nbcase - 1 ) && (M[i+1][j] != -1) ) trouve_zone_rec(M, nbcase, i+1 , j, taille, L);
	if ( (j < nbcase - 1 ) && (M[i][j+1] != -1) ) trouve_zone_rec(M, nbcase, i , j+1, taille, L);		
}


int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff){
  int cpt=0;
	int new_Zsg = M[0][0];
	int taille = 1;
	int tot_cases = dim * dim;

	ListeCase *L = malloc(sizeof(ListeCase));
	if (L==NULL) exit(1);
	init_liste(L);
	trouve_zone_rec(M, dim, 0 , 0, &taille, L);

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

	if (aff == 1) Grille_redessine_Grille();
	if (taille == tot_cases) return cpt; // Condition de fin du jeu : la taille de la Zsg couvre toutes les cases
	
	cpt += sequence_aleatoire_rec(M, G, dim, nbcl, aff);
  return cpt;
}


