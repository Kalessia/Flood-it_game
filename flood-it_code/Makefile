all: Flood-It

API_Gene_instance.o: API_Gene_instance.c API_Gene_instance.h
	gcc -c API_Gene_instance.c `sdl-config --cflags`

API_Grille.o: API_Grille.c API_Grille.h
	gcc -c API_Grille.c `sdl-config --cflags`

Liste_case.o: Liste_case.c Liste_case.h
	gcc -c Liste_case.c `sdl-config --cflags`

Biblio_S_Zsg.o: Biblio_S_Zsg.c Biblio_S_Zsg.h
	gcc -c Biblio_S_Zsg.c `sdl-config --cflags`

Fonctions_exo1.o: Fonctions_exo1.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo1.c `sdl-config --cflags`

Fonctions_exo2.o: Fonctions_exo2.c Entete_Fonctions.h Liste_case.h
	gcc -c Fonctions_exo2.c `sdl-config --cflags`

Fonctions_exo3.o: Fonctions_exo3.c Entete_Fonctions.h Liste_case.h Biblio_S_Zsg.h
	gcc -c Fonctions_exo3.c `sdl-config --cflags`

Graphe.o: Graphe.c Graphe.h
	gcc -c Graphe.c `sdl-config --cflags`

Fonctions_exo5.o: Fonctions_exo5.c Entete_Fonctions.h Liste_case.h Biblio_S_Zsg.h Graphe.h
	gcc -c Fonctions_exo5.c `sdl-config --cflags`

Fonctions_exo6.o: Fonctions_exo6.c Entete_Fonctions.h Liste_case.h Biblio_S_Zsg.h Graphe.h
	gcc -c Fonctions_exo6.c `sdl-config --cflags`

Fonctions_exo7.o: Fonctions_exo7.c Entete_Fonctions.h Liste_case.h Biblio_S_Zsg.h Graphe.h
	gcc -c Fonctions_exo7.c `sdl-config --cflags`

Flood-It.o: Flood-It.c
	gcc -c Flood-It.c `sdl-config --cflags`

Flood-It: Flood-It.o Liste_case.o Biblio_S_Zsg.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo2.o Fonctions_exo3.o Graphe.o Fonctions_exo5.o Fonctions_exo6.o Fonctions_exo7.o
	gcc -o Flood-It Flood-It.o Liste_case.o Biblio_S_Zsg.o API_Grille.o API_Gene_instance.o Fonctions_exo1.o Fonctions_exo2.o Fonctions_exo3.o Graphe.o Fonctions_exo5.o Fonctions_exo6.o Fonctions_exo7.o `sdl-config --libs` 

clean:
	rm -f *.o Flood-It
