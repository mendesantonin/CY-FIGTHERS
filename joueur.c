#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "joueur.h"
#include "entree_sortie.h"

// Affiche la liste des combattants disponibles
void afficher_combattants(Combattant* liste, int taille) {
    for (int i = 0; i < taille; i++) {
        printf("%d - %s (PV: %d, ATQ: %d, DEF: %d, AGI: %d, VIT: %d, Compétences: ",
               i, liste[i].nom, liste[i].pvMax, liste[i].attaque, liste[i].defense,
               liste[i].agilite, liste[i].vitesse);
        for (int k = 0; k < liste[i].nbCompetencesPossedees; k++) {
            printf("%s%s", liste[i].competencesPossedees[k],
                   (k < liste[i].nbCompetencesPossedees - 1) ? ", " : "");
        }
        printf(")\n");
    }
}

// Crée un joueur et lui permet de choisir son équipe
void creer_joueur(Joueur* j, int numeroJoueur, Combattant* liste, int tailleListe, Competence* competences, int nbCompetences) {
    lib_place();
    if (numeroJoueur == 1) while (getchar() != '\n'); // Nettoie l'entrée s'il reste des caractères

    printf("Nom de l'équipe %d : ", numeroJoueur);
    if (!fgets(j->nom, sizeof(j->nom), stdin)) {
        perror("Erreur de lecture du nom");
        exit(EXIT_FAILURE);
    }
    j->nom[strcspn(j->nom, "\n")] = '\0'; // Supprime le \n
    if (strlen(j->nom) == 0) {
        snprintf(j->nom, sizeof(j->nom), "Équipe %d", numeroJoueur);
    }

    printf("\nChoisissez %d combattants parmi la liste suivante :\n", TAILLE_MAX_EQUIPE);
    afficher_combattants(liste, tailleListe);

    j->tailleEquipe = 0;
    while (j->tailleEquipe < TAILLE_MAX_EQUIPE) {
        printf("Choix #%d : ", j->tailleEquipe + 1);
        int choix;
        if (scanf("%d", &choix) != 1 || choix < 0 || choix >= tailleListe) {
            printf("Choix invalide. Veuillez entrer un nombre entre 0 et %d.\n", tailleListe - 1);
            while (getchar() != '\n'); // Vide le tampon
            continue;
        }
        while (getchar() != '\n'); // Vide le tampon

        // Ajout du combattant à l'équipe
        j->equipe[j->tailleEquipe] = liste[choix];
        j->equipe[j->tailleEquipe].barreTour = 0.0f;

        // Initialise la recharge s’il a une compétence
        if (liste[choix].nbCompetencesPossedees > 0) {
            Competence* comp = trouver_competence(competences, nbCompetences, liste[choix].competencesPossedees[0]);
            if (comp) j->equipe[j->tailleEquipe].recharge = comp->delai_rechargement;
        }

        j->tailleEquipe++;
    }
}