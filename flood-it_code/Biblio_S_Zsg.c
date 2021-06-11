#include<stdlib.h>
#include "Biblio_S_Zsg.h"


/* Initialise une structure S_Zsg */
void init_Zsg(S_Zsg *Z, int dim, int nbcl) {
	Z->dim = dim;
	Z->nbcl = nbcl;
	Z->Lzsg = NULL;
	Z->B = NULL;
	Z->App = NULL;
}


/* Ajoute une case dans la liste Lzsg */
int ajoute_Zsg(ListeCase *Lzsg, int i, int j) {
	Elnt_liste *elnt;
  	elnt=(Elnt_liste*) malloc(sizeof(Elnt_liste));
	if (elnt==NULL) return 0;
	elnt->suiv=*Lzsg;
	elnt->i=i;
	elnt->j=j;
	(*Lzsg)=elnt;
}


/* Ajoute une case dans la bordure (liste B) d'une couleur cl donnée ; */
int ajoute_Bordure(ListeCase *B, int i, int j) {
	Elnt_liste *elnt;
  	elnt=(Elnt_liste*) malloc(sizeof(Elnt_liste));
	if (elnt==NULL) return 0;
	elnt->suiv=*B;
	elnt->i=i;
	elnt->j=j;
	(*B)=elnt;
}


/* Renvoie vrai si une case est dans LZsg */
int appartient_Zsg(S_Zsg *Z, int i, int j) {
	if (Z->App[i][j] == -1) return 1;
	return 0;
}


/* Renvoie vrai si une case est dans la bordure (liste B) de couleur cl donnée */
int appartient_Bordure(S_Zsg *Z, int cl, int i, int j) {
	Elnt_liste *cour=(Z->B[cl]);
	while (cour!=NULL){
		if ( (cour->i == i) && (cour->j == j) ) {
			return 1;
		}
		cour=cour->suiv; 
	}
	return 0;
}


/* Desalloue une structure S_Zsg */
void libere_Zsg(S_Zsg *Z) {
	free(Z->Lzsg);
	free(Z->B);
	free(Z->App);
	free(Z);
}



