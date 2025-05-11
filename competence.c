#include "competence.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour charger les données des compétences à partir d'un fichier
int charger_competences(const char *nom_fichier, Competence *competences) {
  FILE *fichier = fopen(nom_fichier, "r");
  if (!fichier) {
    perror("Erreur  lors de l'ouverture du fichier des compétences");
    return 0;
  }
  
  int compteur = 0;
  while (compteur < NBRE_MAX_COMPETENCES &&
         fscanf(fichier, "%49s %d %d %99s %d %d", competences[compteur].nom,
                (int *)&competences[compteur].type,
                &competences[compteur].valeur,
                competences[compteur].description, &competences[compteur].duree,
                &competences[compteur].delai_rechargement) == 6) {  
    // Lecture ligne par ligne tant que le fichier contient des données valides
    compteur++;
  }

  fclose(fichier);
  return compteur;
}
// Fonction pour trouver une compétence par son nom dans le tableau de compétences
Competence *trouver_competence(Competence *competences, int nbCompetences,
                               const char *nom) {
  for (int i = 0; i < nbCompetences; i++) {  
    if (strcmp(competences[i].nom, nom) == 0) {
      return &competences[i]; 
    }
  }
  return NULL;
}
