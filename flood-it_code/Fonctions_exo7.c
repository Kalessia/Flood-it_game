#include<stdio.h>
#include "Entete_Fonctions.h"


void recalcule_fileSommets(Graphe_zone *J, int nStr, int dim, Cellule_som **ListeCellS) {
	int x, y, r, z, k, w;
	Sommet *fils;

	if (nStr==1) {	// 1er appel de parcoursLargeur : a partir de M[0][0] vers M[dim/2][dim-1] 
		x = 0; y = 0;		// coordonnees racine
		r = dim/2; z = dim-1;	// coordonnees destination	
	}
	if (nStr==2) {	// 2eme appel de parcoursLargeur : a partir de M[dim/2][dim-1] vers M[dim-1][dim/2] 
		x = dim/2; y = dim-1;	// coordonnees racine
		r = dim-1; z = dim/3;	// coordonnees destination	
	}
	if (nStr==3) return ;

	for (k=0; k<dim; k++) {
		for (w=0; w<dim; w++) {
			J->mat[k][w]->distance = INFINITO ;
		} }

	//printf("Plus court chemin a partir de M[%d][%d] vers M[%d][%d]\n", x, y, r, z); //TEST
	J->mat[x][y]->pere = NULL;
	parcoursLargeur(J, J->mat[x][y]); // Calcul du parcours entre les nouveaux sommets racine (x,y) et destination (r,z)
	fils = J->mat[r][z];
	while (fils->pere!=NULL) {	  // Reconstruction du plus court chemin, a partir du sommet destination vers la racine
		enfile(fils, ListeCellS); 
		fils = fils->pere;
	}

/*---------------------------------------------------------------------------JEU DE TEST------------
printf("Strategie %d - Parcours a suivre : ", nStr ); afficheListeCellule_som(ListeCellS); printf("\n");
//--------------------------------------------------------------------------------------------------
*/
}

/* Le code BounceStrategyBonus est commmenté seulement dans les parties qui se differencient de l'exo6, pour faciliter la lecture */
int BounceStrategyBonus(Graphe_zone *J, Grille *G, int dim, int nbcl, int aff) {
	int cpt = 0;
	int tot;
	int k;
	int new_Zsg = 0;
	int last_Zsg = J->mat[0][0]->cl;
	int maxNbcase_som;
	int tot_cases_rajoutees_Lzsg = 0;
	int tot_cases = dim * dim;
	int new_Zsg_i, new_Zsg_j;	
	
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

	Elnt_liste *cour;
	
	J->mat[0][0]->marque = 0;
	cour = J->mat[0][0]->cases;
	while (cour != NULL) {					
		ajoute_Bordure(&Z->B[last_Zsg], cour->i, cour->j); 
		cour = cour->suiv;
	}

	tot_cases_rajoutees_Lzsg += agrandit_BordureGraphe(J, Z, 0, 0);
	detruit_liste(&Z->B[J->mat[0][0]->cl]);

	Cellule_som *fileSommets = NULL;

	int nStr = 0; 	// nStr = numero strategia
	Sommet *u;
	while (tot_cases_rajoutees_Lzsg < tot_cases) {

		// D'abord on construit la sequence de couleurs à suivre avec les strategies 1 et 2
		if (nStr < 3) {
			if (fileSommets==NULL) {
				nStr++;
				recalcule_fileSommets(J, nStr, dim, &fileSommets);
			} else {
				u = defile(&fileSommets);
				new_Zsg = u->cl;
				new_Zsg_i = u->cases->i;
				new_Zsg_j = u->cases->j;
			}
		} else {
			// Ensuite on termine le jeu en suivant la strategie strategie_maxBordure de l'exo5.
			maxNbcase_som = 0;
			for (k=0; k<nbcl; k++) {
				tot = 0;
				cour=(Z->B[k]);	
				while (cour!=NULL){
					tot++;
					if (tot > maxNbcase_som) {
						maxNbcase_som = tot; 
						new_Zsg_i = cour->i;
						new_Zsg_j = cour->j;
					}
					cour=cour->suiv;			
				}
			}
			new_Zsg = J->mat[new_Zsg_i][new_Zsg_j]->cl;
		}
		cpt++;

		cour=(Z->Lzsg);
		while (cour!=NULL){
			if (aff == 1)	Grille_attribue_couleur_case(G, cour->i, cour->j, new_Zsg);
    			cour=cour->suiv;
		}

		tot_cases_rajoutees_Lzsg += agrandit_BordureGraphe(J, Z, new_Zsg_i, new_Zsg_j);

		detruit_liste(&Z->B[J->mat[new_Zsg_i][new_Zsg_j]->cl]);

		if (aff == 1)	Grille_redessine_Grille();
		last_Zsg = new_Zsg;
	}
	detruit_liste(&Z->B[new_Zsg]);
	detruit_liste(&Z->Lzsg);
	libere_Zsg(Z);

	return cpt;
}




