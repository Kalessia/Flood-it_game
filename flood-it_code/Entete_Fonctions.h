#ifndef __ENTETE_FONCTIONS__
#define __ENTETE_FONCTIONS__

#include "API_Grille.h"
#include "Liste_case.h"
#include "Biblio_S_Zsg.h"
#include "Graphe.h"



// EXERCICE 1--------------------------------------------------------------------------------------------------------------

/* Retourne dans L la liste des cases de meme couleur que la case i,j et met -1 dans ces cases. VERSION RECURSIVE */
void trouve_zone_rec(int **M, int nbcase,int i, int j, int *taille, ListeCase *L);

/* Algorithme tirant au sort une couleur: il utilise la fonction trouve_zone_rec pour determiner la Zsg.
L'algorithme returne le nombre de tours de jeu effectués. VERSION RECURSIVE */
int sequence_aleatoire_rec(int **M, Grille *G, int dim, int nbcl, int aff);



// EXERCICE 2--------------------------------------------------------------------------------------------------------------

/* Retourne dans L la liste des cases de meme couleur que la case i,j et met -1 dans ces cases. VERSION IMPERATIVE */
void trouve_zone_imp(int **M, int nbcase,int i, int j, int *taille, ListeCase *L);

/* Algorithme tirant au sort une couleur: il utilise la fonction trouve_zone_imp pour determiner la Zsg.
L'algorithme returne le nombre de tours de jeu effectués. VERSION IMPERATIVE*/
int sequence_aleatoire_imp(int **M, Grille *G, int dim, int nbcl, int aff);



// EXERCICE 3--------------------------------------------------------------------------------------------------------------

/* Met à jour les champs Lzsg et B d'une S_Zsg lorsque qu'une case (k,l) de couleur cl, qui est dans la bordure B[cl],
doit basculer dans Lzsg. La fonction retourne le nombre de cases qui a été ajouté à Lzsg */
int agrandit_Zsg(int **M, S_Zsg *Z, int cl, int k, int l);

/* Algorithme tirant au sort une couleur: il utilise la fonction agrandit_Zsg pour determiner la nouvelle bordure B
et agrandire la Zsg. L'algorithme returne le nombre de tours de jeu effectués. */
int sequence_aleatoire_rapide(int **M, Grille *G, int dim, int nbcl, int aff);



// EXERCICE 5--------------------------------------------------------------------------------------------------------------

/* Les fonctions de l'exo5 sont similaires a celles de l'exo3, mais elles s'appliquent à une structure Graphe_zone */

/* Met à jour les champs Lzsg et B d'une S_Zsg lorsque que l'ensemble des cases appartenants à une meme zone (sommet)
de couleur cl (dans la bordure B[cl]) doit basculer dans Lzsg. La fonction retourne le nombre de cases ajouté à Lzsg. */
int agrandit_BordureGraphe(Graphe_zone *J, S_Zsg *Z, int k, int l);

/* L'algorithme utilise la fonction agrandit_BordureGraphe pour determiner la nouvelle bordure B et agrandire la Zsg.
Strategie : a chaque tour de jeu, la nouvelle couleur choisie est celle la plus representée dans la bordure-graphe.
L'algorithme returne le nombre de tours de jeu effectués. */
int strategie_maxBordure(Graphe_zone *J, Grille *G, int dim, int nbcl, int aff);



// EXERCICE 6--------------------------------------------------------------------------------------------------------------

/* L'algorithme calcule le parcours en largeur d'un graphe à partir d'un sommet racine. Pour chaque sommet du graphe,
on stocke le champ distance (distance sommet-racine) et le pere (sommet predecesseur).
En remontant l'arborescence a partir d'un sommet destination, en suivant le pointeur 'pere' vers son predecesseur,
il est possible de reconstruire un plus court chemin entre les sommets racine et destination.*/
void parcoursLargeur(Graphe_zone *J, Sommet *racine) ;

/* L'algorithme utilise la fonction parcoursLargeur pour determiner le plus court chemin entre la premiere (M[0][0])
et la derniere (M[dim-1][dim-1]) case de la grille de jeu. Ce parcours nous permet de trouver la sequence de couleurs minimale
pour colorier la grille en diagonale jusqu'a la case en bas a droite.
Strategie : on colorie en priorité les zones (sommets) correspondantes au plus court chemin du graphe, ensuite on termine le jeu en suivant la strategie strategie_maxBordure de l'exo5. L'algorithme returne le nombre de tours de jeu effectués. */
int strategie_parcoursLargeur_MaxBordure(Graphe_zone *J, Grille *G, int dim, int nbcl, int aff) ;



// EXERCICE 7--------------------------------------------------------------------------------------------------------------

/* Fonction qui recalcule le plus court chemin dans le graphe en appliquant les strategies suivantes:
  - Strategie 1 : Plus court chemin a partir de M[0][0] vers M[dim/2][dim-1]
  - Strategie 2 : Plus court chemin a partir de M[dim/2][dim-1] vers M[dim-1][dim/2] */
void recalcule_fileSommets(Graphe_zone *J, int nStr, int dim, Cellule_som **ListeCellS) ;

/* Algorithme qui utilise la fonction recalcule_fileSommets pour definir la sequence de couleurs a suivre
pendant le deroulement du jeu. Les plus courts chemins selectionnées permettent de colorier la grille selon des axes transversales.
Ensuite on termine le jeu en suivant la strategie strategie_maxBordure de l'exo5. L'algorithme returne le nombre de tours de jeu effectués.
Le code BounceStrategyBonus est commmenté seulement dans les parties qui se differencient de l'exo6, pour faciliter la lecture */
int BounceStrategyBonus(Graphe_zone *J, Grille *G, int dim, int nbcl, int aff) ;



#endif  /* __ENTETE_FONCTIONS__ */


