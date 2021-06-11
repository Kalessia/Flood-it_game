#include<stdio.h>
#include "Entete_Fonctions.h"


void parcoursLargeur(Graphe_zone *J, Sommet *racine) {
	int i;	
	Sommet *u;				// u = sommet predecesseur de v;
	Sommet *v;				// v = sommet successeur de u;  
	Cellule_som *fileSommets = NULL;	// fileSommets = pointeur sur liste chainée de Cellule_som

	racine->distance = 0;
	enfile(racine, &fileSommets);		// On insere la racine dans la liste fileSommets

	Cellule_som *cour;	
	while (fileSommets!=NULL) {		
		u = defile(&fileSommets);	// Pour tout sommet v adjacent de u pas encore visité :
		cour = u->sommet_adj;		// on calcule la distance depuis la racine (= distance_predecesseur +1) 
		while (cour!=NULL) {		// et on enregisre l'adresse du sommet predecesseur u (pere)
			if (cour->sommet->distance == INFINITO) {
				v = cour->sommet;
				v->distance = u->distance + 1;
				v->pere = u;
				enfile(v, &fileSommets);
			}
			cour = cour->suiv;
		}
	}
}


int strategie_parcoursLargeur_MaxBordure(Graphe_zone *J, Grille *G, int dim, int nbcl, int aff) {
	int cpt = 0;
	int tot;
	int k;
	int new_Zsg = 0;
	int last_Zsg = J->mat[0][0]->cl;
	int maxNbcase_som;
	int tot_cases_rajoutees_Lzsg = 0;
	int tot_cases = dim * dim;
	int new_Zsg_i, new_Zsg_j;	
	
	S_Zsg *Z = malloc(sizeof(S_Zsg));		// Allocation et initialisation structure S_Zsg
	if (Z==NULL) exit(1);
	init_Zsg(Z, dim, nbcl);

	Z->Lzsg = malloc(sizeof(ListeCase));		// Allocation et initialisation liste Lzsg
	if (Z->Lzsg==NULL) exit(1);
	init_liste(&Z->Lzsg);	

	Z->B = malloc(nbcl * sizeof(ListeCase));	// Allocation et initialisation bordure-graphe B
	if (Z->B==NULL) exit(1);
	for (k=0; k<nbcl; k++) {
		init_liste(&Z->B[k]);
	}

	Elnt_liste *cour;
	
	J->mat[0][0]->marque = 0;			// Insertion de la premiere zone dans la bordure B[cl]
	cour = J->mat[0][0]->cases;			// Cette premiere insertion assure le fonctionnement de agrandit_BordureGraphe 
	while (cour != NULL) {					
		ajoute_Bordure(&Z->B[last_Zsg], cour->i, cour->j); 
		cour = cour->suiv;
	}

	tot_cases_rajoutees_Lzsg += agrandit_BordureGraphe(J, Z, 0, 0);	// Premier tour de jeu avec la couleur de la case mat[0][0] (cl du sommet pointé)
	detruit_liste(&Z->B[J->mat[0][0]->cl]);

/*---------------------------------------------------------------------------JEU DE TEST------------
int w;									// Deplacer le symbole  '* /'  qui se trouve 
printf("\nContenu Lzsg a l'initialisation : ");				// en bas a gauche (ligne ~85)
afficheListeCase(&Z->Lzsg); printf("\n");				// a la fin de la ligne pointillée au-dessus
printf("Contenu tableau B[cl] a l'initialisation :\n");			// (ligne ~68) pour utiliser ce jeu de test
for (k=0; k<nbcl; k++) {
	printf("B[%d]", k);
	afficheListeCase(&Z->B[k]);
} printf("\n");
printf("Representation du marquage des sommets a l'initialisation :\n");
for (k=0; k<dim; k++) {
	for (w=0; w<dim; w++) {
		printf(" %d ", J->mat[w][k]->marque); } printf("\n"); }
printf("\n");
printf("Tot cases Lzsg a l'initialisation : %d\n\n", tot_cases_rajoutees_Lzsg);
printf("-------------------------------------------------------------------\n\n");
//----------------------------------------------------------------------------------------------------
*/

	// On appelle la fonction parcoursLargeur dans le but de calculer le plus court chemin
	// entre la premiere (M[0][0]) et la derniere (M[dim-1][dim-1]) case de la grille de jeu.
	Cellule_som *fileSommets = NULL;	
	parcoursLargeur(J, J->mat[0][0]);

	Sommet *fils = J->mat[dim-1][dim-1];
	while (fils->pere!=NULL){
		enfile(fils, &fileSommets);
		fils = fils->pere;
	}
	//afficheListeCellule_som(&fileSommets); // JEU DE TEST

	Sommet *u;
	while (tot_cases_rajoutees_Lzsg < tot_cases) {
		// Nouveau tour de jeu : choix d'une nouvelle couleur, selon 2 strategies differentes:
		//   - d'abord on utilise la sequence de couleurs donnée par le plus court chemin, afin de
		//     colorier le plus rapidement possible la diagonale de la grille
		//   - en suite on utilise la strategie strategie_maxBordure de l'exo5
		if (fileSommets!=NULL) {
			u = defile(&fileSommets);
			new_Zsg = u->cl;
			new_Zsg_i = u->cases->i;
			new_Zsg_j = u->cases->j;
		} else	{
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

		//On affecte la nouvelle couleur new_Zsg à toute case enregistrée dans la liste Lzsg
		cour=(Z->Lzsg);
		while (cour!=NULL){
			if (aff == 1)	Grille_attribue_couleur_case(G, cour->i, cour->j, new_Zsg);
    			cour=cour->suiv;
		}

		// On appelle agrandit_BordureGraphe pour incorporer les cases dans B[new_Zsg] en Lzsg
		// et calculer la nouvelle bordure-graphe en ajoutant les cases que la composent a B[cl de la case].
		tot_cases_rajoutees_Lzsg += agrandit_BordureGraphe(J, Z, new_Zsg_i, new_Zsg_j);

		// On elimine la liste B[cl] contenant les elements qu'on vient d'ajouter a Lzsg
		detruit_liste(&Z->B[J->mat[new_Zsg_i][new_Zsg_j]->cl]);

/*---------------------------------------------------------------------------JEU DE TEST------------
printf("La couleur courante etait : %d \n", last_Zsg);
printf("La nouvelle couleur pour ce tour de jeu sera %d (zone de %d cases)\n\n", new_Zsg, maxNbcase_som);
printf("Contenu Lzsg:\n");
afficheListeCase(&Z->Lzsg); printf("\n");
printf("Contenu tableau B[cl]:\n");
for (k=0; k<nbcl; k++) {
	printf("B[%d]", k);
	afficheListeCase(&Z->B[k]);
} printf("\n");
printf("Representation du marquage des sommets :\n");
for (k=0; k<dim; k++) {
	for (w=0; w<dim; w++) {
		printf(" %d ", J->mat[w][k]->marque); } printf("\n"); }
printf("\n");
printf("Tot cases Lzsg : %d\n\n", tot_cases_rajoutees_Lzsg);
printf("-------------------------------------------------------------------\n\n");
//----------------------------------------------------------------------------------------------------
*/
		if (aff == 1)	Grille_redessine_Grille();
		last_Zsg = new_Zsg;
	}
	detruit_liste(&Z->B[new_Zsg]);
	detruit_liste(&Z->Lzsg);
	libere_Zsg(Z);

	return cpt;
}




