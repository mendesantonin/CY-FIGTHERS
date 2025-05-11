#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "jeu.h"
#include "entree_sortie.h"
#include "joueur.h"
#include "combattant.h"
#include "competence.h"

int main() {
    srand(time(NULL));

    printf("=== BIENVENUE SUR POKEMON CY ===\n\n");
    pause();// Pause dans le programme
    
    // Chargement des combattants depuis un fichier
    Combattant combattants[MAX_COMBATTANTS];
    int nbCombattants = charger_combattants("donnees/combattants.txt", combattants);
    if (nbCombattants <= 0) {
        printf("Erreur lors du chargement des combattants.\n");
        return 1;
    }

    // Chargement des compétences depuis un fichier
    Competence competences[NBRE_MAX_COMPETENCES];
    int nbCompetences = charger_competences("donnees/competences.txt", competences);
    if (nbCompetences <= 0) {
        printf("Erreur lors du chargement des compétences.\n");
        return 1;
    }

    int mode;
    do {
        lib_place();  // Nettoie l’écran avant affichage

        // Affichage du menu principal
        printf("Choisissez le mode de jeu :\n");
        printf("1. Joueur vs Joueur\n");
        printf("2. Joueur vs IA\n");
        printf("3. Quitter\n");
        printf("Votre choix : ");
        scanf("%d", &mode);

        Joueur joueur1, joueur2;// Création des joueurs

        switch (mode) {
        case 1:  // Mode Joueur vs Joueur
            creer_joueur(&joueur1, 1, combattants, nbCombattants, competences, nbCompetences);
            creer_joueur(&joueur2, 2, combattants, nbCombattants, competences, nbCompetences);
            demarrer_combat(&joueur1, &joueur2, competences, nbCompetences, -1);
            break;

        case 2:  // Mode Joueur vs IA
            creer_joueur(&joueur1, 1, combattants, nbCombattants, competences, nbCompetences);
            creer_joueur(&joueur2, 2, combattants, nbCombattants, competences, nbCompetences);
            int difficulte;
            do {
                printf("Choisissez la difficulté de l'IA :\n");
                printf("1. Noob\n2. Facile\n3. Moyen\nVotre choix : ");
                scanf("%d", &difficulte);
            } while (difficulte < 1 || difficulte > 3);
            demarrer_combat(&joueur1, &joueur2, competences, nbCompetences, difficulte);
            break;

        case 3:
            printf("====================================\n");
            printf("    Merci d’avoir joué à Pokemon CY\n");
            printf("====================================\n");
            printf("           💥 À bientôt ! 💥\n");
            printf("====================================\n\n");
            break;

        default:  // Entrée invalide
            printf("Choix invalide.\n");
            break;
        }

        pause();
    } while (mode != 3);

    return 0;
}