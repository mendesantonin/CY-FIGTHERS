#ifndef JEU_H
#define JEU_H

#include "joueur.h"
#include "competence.h"


void demarrer_combat(Joueur* j1, Joueur* j2, Competence* competences, int nbCompetences, int difficulte);
void attaquer(Combattant* attaquant, Combattant* cible, Competence* competences, int nbCompetences, Joueur* attaquantEquipe, Joueur* defenseurEquipe);
void utiliser_competence(Combattant* attaquant, Combattant* cible, Competence* competence);
Combattant* choisir_cible(Combattant* attaquant, Joueur* defenseur, Joueur* Eattaquant);
void afficher_equipe(Joueur* j);
int tous_ko(Joueur* j);
void tour_ia(Joueur* ia, Joueur* adversaire, int niveau, Competence* competences, int nbCompetences);

#endif 