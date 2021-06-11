#ifndef __Biblio_S_ZSG__
#define __Biblio_S_ZSG__
#include "Liste_case.h"


/* Structure S_Zsg */
typedef struct {
	int dim;		/* dimension de la grille */
	int nbcl;		/* nombre de couleurs */
	ListeCase Lzsg;		/* liste de cases de la zone Zsg */
	ListeCase *B;		/* tableau de listes de cases de la bordure reparties par couleurs, de taille nbcl */
				/* chaque case B[c] est une liste contenant les cases de couleur c de la bordure */
	int **App;		/* tableau n x n a double entree des appartenances */
} S_Zsg;


/* Initialise une structure _Zsg */
void init_Zsg(S_Zsg *Z, int dim, int nbcl);

/* Ajoute une case dans la liste Lzsg */
int ajoute_Zsg(ListeCase *Lzsg, int i, int j);

/* Ajoute une case dans la bordure d'une couleur cl donnée ; */
int ajoute_Bordure(ListeCase *B, int i, int j);

/* Renvoie vrai si une case est dans LZsg */
int appartient_Zsg(S_Zsg *Z, int i, int j);

/* Renvoie vrai si une case est dans la bordure de couleur cl donnée */
int appartient_Bordure(S_Zsg *Z, int cl, int i, int j);

/* Desalloue une structure S_Zsg */
void libere_Zsg(S_Zsg *Z);


#endif
