#ifndef COMPETENCE_H
#define COMPETENCE_H

#define NBRE_MAX_COMPETENCES 100
#define LONGUEUR_MAX_NOM 50
#define LONGUEUR_MAX_DESCRIPTION 100
#define MAX_EFFETS 10

typedef enum {
    COMPETENCE_DEGAT,
    COMPETENCE_SOIN,
    COMPETENCE_BONUS,
    COMPETENCE_MALUS
} TypeCompetence;

typedef struct {
    char nom[LONGUEUR_MAX_NOM];
    TypeCompetence type;
    int valeur;
    char description[LONGUEUR_MAX_DESCRIPTION];
    int duree;
    int delai_rechargement;
} Competence;

typedef struct {
    TypeCompetence type;
    int valeur;
    int duree_restante;
} EffetActif;

int charger_competences(const char* nom_fichier, Competence* competences);
Competence* trouver_competence(Competence* competences, int nbCompetences, const char* nom);

#endif 
