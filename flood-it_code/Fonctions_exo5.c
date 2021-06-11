#include<stdio.h>
#include "Entete_Fonctions.h"


int agrandit_BordureGraphe(Graphe_zone *J, S_Zsg *Z, int k, int l) {
	int tot_cases_rajoutees_Lzsg = 0;

	Elnt_liste *cour = (Z->B[J->mat[k][l]->cl]);	// On parcourt la liste B[cl] du meme couleur que la case passée en parametre
	Cellule_som *sondaAdj;
	Elnt_liste *sondaCases;

	// On ajoute a la liste Lzsg toutes les cases contenues dans la liste B[cl]	
	while (cour!=NULL){
		ajoute_Zsg(&Z->Lzsg, cour->i, cour->j);
		J->mat[cour->i][cour->j]->marque = 0;	// La marque '0' signifie que le sommet correspondant fait partie de la Zsg
		tot_cases_rajoutees_Lzsg++;
					
		// Pour toute nouvelle case acquise dans Lzsg (et relatif sommet dans J->mat) :
		//   - on explore la liste de sommets adjacents 'sommet_adj'. Pour chacun de ceux la :
		//   - on explore la liste de cases 'cases' (et relatif sommet dans J->mat). Si le sommet n'a pas encore ete visite,
		//     (marque != 0) alors on ajoute sa case a la bordure-graphe B[cl] de son couleur.
		// On met à jour la marque à 1 pour tous les sommets ajoutés à la bordure-graphe.
		sondaAdj = J->mat[cour->i][cour->j]->sommet_adj;
		while (sondaAdj != NULL) {						
			sondaCases = sondaAdj->sommet->cases;
			while (sondaCases != NULL) {	
				if (J->mat[sondaCases->i][sondaCases->j]->marque != 0) { 
					if (!appartient_Bordure(Z, J->mat[sondaCases->i][sondaCases->j]->cl, sondaCases->i, sondaCases->j) ) {	
						ajoute_Bordure(&Z->B[J->mat[sondaCases->i][sondaCases->j]->cl], sondaCases->i, sondaCases->j); 
						J->mat[sondaCases->i][sondaCases->j]->marque = 1;
					}
				}
				sondaCases = sondaCases->suiv;
			}
			sondaAdj = sondaAdj->suiv;
		}
		cour=cour->suiv;
	}
	return tot_cases_rajoutees_Lzsg;
}


int strategie_maxBordure(Graphe_zone *J, Grille *G, int dim, int nbcl, int aff) {
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

	tot_cases_rajoutees_Lzsg += agrandit_BordureGraphe(J, Z, 0, 0); // Premier tour de jeu avec la couleur de la case mat[0][0] (cl du sommet pointé)
	detruit_liste(&Z->B[J->mat[0][0]->cl]);

/*---------------------------------------------------------------------------JEU DE TEST------------
int w;									// Deplacer le symbole  '* /'  qui se trouve 
printf("\nContenu Lzsg a l'initialisation : ");				// en bas a gauche (ligne ~97)
afficheListeCase(&Z->Lzsg); printf("\n");				// a la fin de la ligne pointillée au-dessus
printf("Contenu tableau B[cl] a l'initialisation :\n");			// (ligne ~80) pour utiliser ce jeu de test
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
	while (tot_cases_rajoutees_Lzsg < tot_cases) {
		// Nouveau tour de jeu : choix d'une nouvelle couleur, correspondant à la couleur plus representee dans la bordure
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
		cpt++;

		// On affecte la nouvelle couleur new_Zsg à toute case enregistrée dans la liste Lzsg
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



