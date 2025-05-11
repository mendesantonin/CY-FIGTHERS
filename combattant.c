#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "combattant.h"

// Fonction pour charger les données des combattants à partir d'un fichier
// Retourne le nombre de combattants chargés

int charger_combattants(const char* nomFichier, Combattant* combattants) {
    FILE* fichier = fopen(nomFichier, "r");// Ouverture du fichier en mode lecture
    if (!fichier) {
        perror("Erreur lors de l'ouverture du fichier des combattants");
        return 0;
    }

    int compteur = 0;
    while (compteur < MAX_COMBATTANTS && !feof(fichier)) {// Lecture des données d'un combattant à partir d'une ligne du fichier
        char competencesStr[200];
        int result = fscanf(fichier, "%s %d %d %d %d %d %d %[^\n]",
                            combattants[compteur].nom,
                            &combattants[compteur].pvMax,
                            &combattants[compteur].pvActuels,
                            &combattants[compteur].attaque,
                            &combattants[compteur].defense,
                            &combattants[compteur].agilite,
                            &combattants[compteur].vitesse,
                            competencesStr);
        if (result != 8) break;

        // Séparer les compétences
        char* token = strtok(competencesStr, ";");
        int k = 0;
        while (token && k < MAX_COMPETENCES_PAR_COMBATTANT) {
            strcpy(combattants[compteur].competencesPossedees[k++], token);// Copie de chaque compétence dans le tableau du combattant
            token = strtok(NULL, ";");
        }
        combattants[compteur].nbCompetencesPossedees = k;

        combattants[compteur].recharge = 0;
        combattants[compteur].nbEffets = 0;

        compteur++;
    }

    fclose(fichier);
    return compteur;
}