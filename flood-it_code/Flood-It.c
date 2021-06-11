#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "API_Gene_instance.h"
#include "Entete_Fonctions.h"
 
 
int main(int argc,char**argv){

  int dim, nbcl, nivdif, graine, exo, aff;
  Grille *G;
  int i,j;
  int **M;

  clock_t
    temps_initial, /* Temps initial en micro-secondes */
    temps_final;   /* Temps final en micro-secondes */
  float
    temps_cpu;     /* Temps total en secondes */ 
 


  if(argc!=7){
    printf("usage: %s <dimension> <nb_de_couleurs> <niveau_difficulte> <graine> <exo:0-1-2> <aff 0/1>\n",argv[0]);
    return 1;
  }

  dim=atoi(argv[1]);
  nbcl=atoi(argv[2]);
  nivdif=atoi(argv[3]);
  graine=atoi(argv[4]);
  exo=atoi(argv[5]);
  aff=atoi(argv[6]);

  /* Allocation puis Generation de l'instance */
  M=(int **) malloc(sizeof(int*)*dim);
  for (i=0;i<dim;i++){
    M[i]=(int*) malloc(sizeof(int)*dim);
    if (M[i]==0) printf("Pas assez d'espace mémoire disponible\n");
  }

  Gene_instance_genere_matrice(dim, nbcl, nivdif, graine, M);
 

  if (aff==1){  /* Affichage de la grille */
    Grille_init(dim,nbcl, 500,&G);

    Grille_ouvre_fenetre(G);

    for (i=0;i<dim;i++)
      for (j=0;j<dim;j++){
	Grille_attribue_couleur_case(G,i,j,M[i][j]);
      }

    Grille_redessine_Grille();
    Grille_attente_touche();
  }

  temps_initial = clock ();


//-------------------- FLOOD IT PARTIE 1 ------------------------------------------------------------------
  
	if (exo==0){
		printf("%d essais : ", sequence_aleatoire_rec(M, G, dim, nbcl, aff)); 
	}

  	if (exo==1){
    	 	printf("%d essais : ", sequence_aleatoire_imp(M, G, dim, nbcl, aff)); 
  	}

	if (exo==2){
    	 	printf("%d essais : ", sequence_aleatoire_rapide(M, G, dim, nbcl, aff)); 
  	}

//-------------------- FLOOD IT PARTIE 2 ------------------------------------------------------------------

	Graphe_zone *J;

	if (exo==4){
		J = cree_graphe_zone(M, dim);
		afficheGraphe(J);
		libereGraphe(J);
  	}

	if (exo==5){
		J = cree_graphe_zone(M, dim);
		printf("%d essais : ", strategie_maxBordure(J, G, dim, nbcl, aff));
		libereGraphe(J);
  	}

	if (exo==6){
		J = cree_graphe_zone(M, dim);
		printf("%d essais : ", strategie_parcoursLargeur_MaxBordure(J, G, dim, nbcl, aff));
		libereGraphe(J);  	 	
  	}

	if (exo==7){
		J = cree_graphe_zone(M, dim);
		printf("%d essais : ", BounceStrategyBonus(J, G, dim, nbcl, aff));
		libereGraphe(J);
  	}

//---------------------------------------------------------------------------------------------------------

  temps_final = clock ();
  temps_cpu = (temps_final - temps_initial) * 1e-6;
	
  if (exo!=4) printf(" %f\n",temps_cpu);

/*
	// Comparaison en fonction du nombre de cases
	FILE *f = NULL;
	f = fopen("Courbes_nbcases.txt", "a+");
	if ( !f ) fprintf (stderr, "Impossible ouvrir le fichier 'Courbes_nbcases.txt'" );  
        else fprintf ( f, "%d %d\n", dim, temps_cpu );
	fclose(f);
	
	// Comparaison en fonction du nombre de couleurs
	FILE *g = NULL;
	g = fopen("Courbes_couleurs.txt", "a+");
	if ( !g ) fprintf (stderr, "Impossible ouvrir le fichier 'Courbes_couleurs.txt'" );  
        else fprintf ( g, "%d %d\n", nbcl, temps_cpu );
	fclose(g);
*/


  /* Desallocation de la matrice */
  for(i = 0; i< dim; i++) {
    if (M[i])
        free(M[i]);
  }
  if (M) free(M);

  
 
  if (aff==1){  /* Fermeture et désallocation de la grille */
    Grille_ferme_fenetre();
    Grille_free(&G);
  }

  return 0;
}
