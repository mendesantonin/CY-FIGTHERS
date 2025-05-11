all : exec
main.o : main.c jeu.h entree_sortie.h joueur.h combattant.h competence.h
	gcc -c main.c -o main.o
competence.o : competence.c competence.h
	gcc -c competence.c -o competence.o
joueur.o : joueur.c joueur.h
	gcc -c joueur.c -o joueur.o
entree_sortie.o : entree_sortie.c entree_sortie.h
	gcc -c entree_sortie.c -o entree_sortie.o
jeu.o : jeu.c jeu.h entree_sortie.h combattant.h
	gcc -c jeu.c -o jeu.o
combattant.o : combattant.c combattant.h
	gcc -c combattant.c -o combattant.o
exec : main.o competence.o joueur.o entree_sortie.o jeu.o combattant.o
	gcc main.o competence.o joueur.o entree_sortie.o jeu.o combattant.o -o exec
