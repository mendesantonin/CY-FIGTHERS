#ifndef JOUEUR_H
#define JOUEUR_H

#include "combattant.h"
#include "competence.h"

#define TAILLE_MAX_EQUIPE 3

typedef struct {
    char nom[50];
    Combattant equipe[TAILLE_MAX_EQUIPE];
    int tailleEquipe;
} Joueur;

void creer_joueur(Joueur* j, int numeroJoueur, Combattant* liste, int tailleListe, Competence* competences, int nbCompetences);

#endif 
