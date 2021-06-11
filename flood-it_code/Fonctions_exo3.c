#include<stdio.h>
#include "Entete_Fonctions.h"


int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l) {
	int tot_cases_rajoutees_Lzsg = 0;
	int i;
	int j;

	ListeCase *P = malloc(sizeof(ListeCase));
	if (P==NULL) exit(1);
	init_liste(P);
	Elnt_liste *cour=(*P);	
	ajoute_en_tete(P, k, l);

	while (!test_liste_vide(P)) { 		// Au debut, la pile P ne contient que la case (0,0)
		i=(*P)->i; j= (*P)->j; 		// On examine le dernier element ajouté à P
		if (M[i][j] == cl) {				// Si la case examinée est de couleur cl
			if (!appartient_Zsg(Z, i, j)) {		// et si elle n'appartient pas déjà à Lzsg
				ajoute_Zsg(&Z->Lzsg, i, j);	// On ajoute la case i,j à la liste Lzsg 
				Z->App[i][j] = -1 ;
				tot_cases_rajoutees_Lzsg++;

				// On collecte les cases adjacentes à la Zsg pas encore traitées
				if ( (i != 0) && (M[i-1][j] != -1) ) {
					enleve_en_tete(P, &i, &j);
					ajoute_en_tete(P, i-1, j);
					ajoute_en_tete(P, i, j);
				}	 
				if ( (j != 0) && (M[i][j-1] != -1) ) {
					enleve_en_tete(P, &i, &j);
					ajoute_en_tete(P, i, j-1);
					ajoute_en_tete(P, i, j);
				}
				if ( (i < Z->dim - 1 ) && (M[i+1][j] != -1) ) {
					enleve_en_tete(P, &i, &j);
					ajoute_en_tete(P, i+1, j);
					ajoute_en_tete(P, i, j);
				}
				if ( (j < Z->dim - 1 ) && (M[i][j+1] != -1) ) {
					enleve_en_tete(P, &i, &j);
					ajoute_en_tete(P, i, j+1);
					ajoute_en_tete(P, i, j);	
				}
			}
		} else { // Sinon, si elle n'appartient pas déjà à B[cl], on ajoute la case i,j à la liste B[cl]
			if (!appartient_Bordure(Z, cl, i, j) && Z->App[i][j] != -1)  {
				ajoute_Bordure(&Z->B[cl], i, j);
				Z->App[i][j] = M[i][j];
			}
		}
		
	// On elimine de la pile l'element qu'on vient de traiter
	enleve_en_tete(P, &i, &j);	
	}

	detruit_liste(P);
	free(P);

	return tot_cases_rajoutees_Lzsg;
}


int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff) {
	int cpt = 0;
	int k, w;
	int new_Zsg = M[0][0];
	int last_Zsg;
	int tot_cases_rajoutees_Lzsg = 0;
	int tot_cases = dim * dim;
	
	S_Zsg *Z = malloc(sizeof(S_Zsg));
	if (Z==NULL) exit(1);
	init_Zsg(Z, dim, nbcl);

	Z->Lzsg = malloc(sizeof(ListeCase));
	if (Z->Lzsg==NULL) exit(1);
	init_liste(&Z->Lzsg);	

	Z->B = malloc(nbcl * sizeof(ListeCase));
	if (Z->B==NULL) exit(1);
	for (k=0; k<nbcl; k++) {
		init_liste(&Z->B[k]);
	}

	Z->App = (int**) malloc(dim * sizeof(int*)); 		// Valeurs possibles pour App: -2, -1 ou cl
	if (Z->App==NULL) exit(1);				// <-2>  signifie "case ni dans LZsg, ni dans B"
	for (k=0; k<dim; k++) {					// <-1>  signifie "case appartenant à LZsg"
		Z->App[k] = (int*) malloc(dim * sizeof(int));	// <cl>  signifie "case appartenant à B[cl]" 
		if (Z->App[k]==NULL) exit(1); }
	for (k=0; k<dim; k++) {
		for (w=0; w<dim; w++) {
			Z->App[k][w] = -2; } } 			// Initialisation de Z->App à -2
						

	Elnt_liste *cour;
	
	tot_cases_rajoutees_Lzsg += agrandit_Zsg(M, Z, new_Zsg, 0, 0);	// valeurs initiales des listes Lzsg et B

	while (tot_cases_rajoutees_Lzsg != tot_cases) {
		//Nouvel tour de jeu : choix d'une nouvelle couleur, correspondant à un indice aleatoire 
		last_Zsg = M[0][0];
		while ( new_Zsg == last_Zsg) {
			new_Zsg = rand()%(nbcl);
		}
		cpt++;

		//On affecte la nouvelle couleur new_Zsg à toute case enregistrée dans la liste Lzsg
		cour=(Z->Lzsg);
		while (cour!=NULL){
			M[cour->i][cour->j] = new_Zsg; 
			if (aff == 1)	Grille_attribue_couleur_case(G, cour->i, cour->j, M[cour->i][cour->j]);
    			cour=cour->suiv;
		}

		cour=(Z->B[last_Zsg]);
		while (cour!=NULL){
			tot_cases_rajoutees_Lzsg += agrandit_Zsg(M, Z, new_Zsg, cour->i, cour->j);
   			cour=cour->suiv;
		}
		detruit_liste(&Z->B[last_Zsg]);

		if (aff == 1)	Grille_redessine_Grille();
	}
	detruit_liste(&Z->B[new_Zsg]);
	detruit_liste(&Z->Lzsg);
	for(k=0; k<dim; k++) {
		if (Z->App[k]) free(Z->App[k]);
	}
	libere_Zsg(Z);
	
	return cpt;
}


