#ifndef COMBATTANT_H
#define COMBATTANT_H

#include "competence.h"

#define MAX_NOM_LEN 50
#define MAX_COMBATTANTS 100
#define MAX_COMPETENCES_PAR_COMBATTANT 3

typedef struct {
    char nom[MAX_NOM_LEN];
    int pvMax;
    int pvActuels;
    int attaque;
    int defense;
    int agilite;
    int vitesse;
    int recharge;
    EffetActif effets[MAX_EFFETS];
    int nbEffets;
    char competencesPossedees[MAX_COMPETENCES_PAR_COMBATTANT][LONGUEUR_MAX_NOM];
    int nbCompetencesPossedees;
    float barreTour;
} Combattant;

int charger_combattants(const char* nomFichier, Combattant* combattants);

#endif // COMBATTANT_H
