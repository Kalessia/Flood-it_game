#ifndef __GRAPHE__
#define __GRAPHE__

#define INFINITO 2140000000


typedef struct sommet Sommet;

/* Element d'une liste chainee de pointeurs sur Sommets */
typedef struct cellule_som {
	Sommet * sommet;
	struct	cellule_som * suiv;
} Cellule_som;


struct sommet {
	int num;			/* Numero du sommet (sert uniquement a l'affichage) */
	int cl;				/* Couleur d'origine du sommet-zone */
	ListeCase  cases;		/* Listes des cases du sommet-zone */
	int nbcase_som;			/* Nombre de cases de cette liste */
	Cellule_som * sommet_adj;	/* Liste des areteses pointeurs sur les sommets adjacents */
	int marque; // pour EXO5	/* Statut du sommet : 0 si dans Zsg, 1 si dans Bordure Zsg, 2 si non visité */
	int distance; // pour EXO6	/* nombre d'aretes reliant ce sommet a la racine */
	Sommet *pere; // pour EXO6	/* pere du sommet dans l'arborescence du parcours en largeur */
};


typedef struct graphe_zone {
	int nbsom;		/* Nombre de sommets dans le graphe */
	int dimM;		/* Dimension de la matrice M representée et de la matrice mat */
	Cellule_som *som;	/* Liste chainee des sommets du graphe */
	Sommet  ***mat;		/* Matrice de pointeurs sur les sommets indiquant a quel sommet appartient une case (i,j) de la grille */
} Graphe_zone;



/* Fonction qui alloue et construit une structure Graphe_zone a partir des informations contenues dans la matrice M.
Chaque sommet representera une zone de cases adjacentes dans M caracterisées par la meme couleur. */
Graphe_zone* cree_graphe_zone(int **M, int dim);

/* Initialise une structure Graphe_zone */
void initGraphe(Graphe_zone *J, int dim) ;

/* Initialise une structure Sommet */
void initSommet(Sommet *s, int clM, int incr) ;

/* Fonction qui ajoute une nouvelle Cellule_som contenant le sommet s à la liste passée en parametre.
Insertion à la fin pour garantir l'ordre des sommets dans l'affichage */
void ajoute_liste_sommet(Sommet *s, Cellule_som **ListeCellS) ;

/* Detruit tous les elements d'une liste de Cellule_som passée en parametre et reinitialise la liste a NULL.
NB: la fonction ne detruit pas les sommets referenciés par les Cellule_som */
void detruit_liste_somme(Cellule_som **ListeCellS) ;

/* Creation d'une nouvelle Cellule_som representant une nouvelle arete entre s1 et s2.
La fonction met à jour s1 en ajoutant s2 a la liste sommet_adj de s1 */
void ajoute_voisin(Sommet *s1, Sommet *s2) ;

/* La fonction retourne 1 si s1 et s2 sont adjacents, 0 sinon */
int sont_ils_adjacents(Sommet *s1, Sommet *s2) ;

/* Affichage des sommets contenus dans une liste de Cellule_som */
void afficheListeCellule_som(Cellule_som **ListeCellS) ;

/* Affichage d'un graphe_zone */
void afficheGraphe(Graphe_zone *J) ;

/* Insertion en tete d'une nouvelle Cellule_som à une liste de Cellule_som passée en parametre. */
void enfile(Sommet *s, Cellule_som **ListeCellS) ;

/* La fonction retourne le premier element en tete d'une liste de Cellule_som passée en parametre.
La Cellule_som contenant le sommet s est liberee. */
Sommet* defile(Cellule_som **ListeCellS) ;

/* Desalloue une structure Graphe_zone */
void libereGraphe(Graphe_zone *J) ;



#endif  /* __GRAPHE__ */



