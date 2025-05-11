#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jeu.h"
#include "entree_sortie.h"
#include "combattant.h"



// Fonction pour vérifier si tous les combattants d'un joueur sont KO
int tous_ko(Joueur* j) {
    for (int i = 0; i < j->tailleEquipe; i++) {
        if (j->equipe[i].pvActuels > 0) return 0;
    }
    return 1;
}

// Fonction pour afficher une barre de progression
void afficher_barre(float pourcentage) {
    int largeur = 20;  // longueur totale de la barre en caractères
    int remplissage = (int)((pourcentage / 100.0f) * largeur);
    printf("[");
    for (int i = 0; i < largeur; i++) {
        if (i < remplissage)
            printf("█");  
        else
            printf(" ");  
    }
    printf("] %.0f%%", pourcentage);
}

// Fonction pour afficher l'équipe d'un joueur
void afficher_equipe(Joueur* j) {
    printf("\n=== ÉQUIPE DE %s ===\n", j->nom);
    for (int i = 0; i < j->tailleEquipe; i++) {
        Combattant c = j->equipe[i];
        printf("%d. %s %s | PV: %d/%d | ATQ: %d | DEF: %d | recharge : %d tour(s)\n",
            i + 1, c.nom, c.pvActuels <= 0 ? "💀" : "", c.pvActuels, c.pvMax, c.attaque, c.defense, c.recharge);
        printf("   Barre de tour : ");
        afficher_barre(c.barreTour);
        printf("\n");
    }
    printf("=====================\n");
}
 
// Fonction pour choisir une cible parmi les combattants d'un joueur
Combattant* choisir_cible(Combattant* attaquant, Joueur* defenseur, Joueur* Eattaquant) {
    printf("\n%s de l’équipe %s prépare son attaque (PV: %d)\n", attaquant->nom, Eattaquant->nom, attaquant->pvActuels);
    printf("Choisissez une cible parmi l'équipe %s :\n", defenseur->nom);
    for (int i = 0; i < defenseur->tailleEquipe; i++) {
        printf("%d - %s %s (PV: %d/%d)\n", i, defenseur->equipe[i].nom,
               defenseur->equipe[i].pvActuels <= 0 ? "💀" : "", defenseur->equipe[i].pvActuels, defenseur->equipe[i].pvMax);
    }
    int choix;
    do {
        printf("Votre choix : ");
        scanf("%d", &choix);
        // Si le choix est invalide
        if (choix < 0 || choix >= defenseur->tailleEquipe) {
            printf("❌ Choix invalide. Veuillez entrer un numéro valide.\n");
        }
        // Si la cible est KO
        else if (defenseur->equipe[choix].pvActuels <= 0) {
            printf("⚠️ Ce combattant est déjà KO. Choisissez un autre.\n");
        }
        // Vérifie que le choix est valide et que le combattant ciblé est en vie
    } while (choix < 0 || choix >= defenseur->tailleEquipe || defenseur->equipe[choix].pvActuels <= 0);
    return &defenseur->equipe[choix];
}

// Met à jour la recharge d'un combattant
void mise_a_jour_recharge(Combattant* c) {
    if (c->recharge > 0) c->recharge--; 
}

// Applique les effets actifs sur un combattant
void appliquer_effets(Combattant* c) {
    for (int i = 0; i < c->nbEffets; i++) {
        EffetActif* e = &c->effets[i];
        // Vérifie si c'est le dernier tour avant suppression
        if (e->duree_restante == 1) {
            if (e->type == COMPETENCE_BONUS) c->attaque -= e->valeur;
            else if (e->type == COMPETENCE_MALUS) c->attaque += e->valeur;
        }
        e->duree_restante--;
    }
    // Nettoyage des effets expirés
    int j = 0;
    for (int i = 0; i < c->nbEffets; i++) {
        if (c->effets[i].duree_restante > 0) c->effets[j++] = c->effets[i];
    }
    c->nbEffets = j;
}

// Utilise une compétence sur un combattant ciblé
void utiliser_competence(Combattant* attaquant, Combattant* cible, Competence* competence) {
    if (!competence) {
        printf("Erreur : compétence introuvable !\n");
        return;
    }
    switch (competence->type) {
        case COMPETENCE_DEGAT: 
            cible->pvActuels -= competence->valeur;
            if (cible->pvActuels < 0) cible->pvActuels = 0;
            printf("%s utilise %s : %d dégâts infligés à %s !\n",
                   attaquant->nom, competence->nom, competence->valeur, cible->nom);
            break;
        case COMPETENCE_SOIN:
            attaquant->pvActuels += competence->valeur;
            if (attaquant->pvActuels > attaquant->pvMax) attaquant->pvActuels = attaquant->pvMax;
            printf("%s utilise %s : %d PV soignés !\n", attaquant->nom, competence->nom, competence->valeur);
            break;
        case COMPETENCE_BONUS:
            if (attaquant->nbEffets < MAX_EFFETS) {
                attaquant->attaque += competence->valeur;  
                attaquant->effets[attaquant->nbEffets++] = (EffetActif){COMPETENCE_BONUS, competence->valeur, competence->duree};
            }
            printf("%s utilise %s : +%d attaque pendant %d tours !\n",
                   attaquant->nom, competence->nom, competence->valeur, competence->duree);
            break;
        case COMPETENCE_MALUS:
            if (cible->nbEffets < MAX_EFFETS) {
                cible->attaque -= competence->valeur;  
                cible->effets[cible->nbEffets++] = (EffetActif){COMPETENCE_MALUS, competence->valeur, competence->duree};
            }
            printf("%s utilise %s : -%d attaque pour %s pendant %d tours !\n",
                   attaquant->nom, competence->nom, competence->valeur, cible->nom, competence->duree);
            break;
    }
    attaquant->recharge = competence->delai_rechargement;
}

// Fonction pour gérer l'attaque d'un combattant sur un autre
void attaquer(Combattant* attaquant, Combattant* cible, Competence* competences, int nbCompetences, Joueur* attaquantEquipe, Joueur* defenseurEquipe) {
    int choix;
    
    printf("\n---- Actions de %s ----\n", attaquant->nom);
    printf("1. Attaque normale\n");
    if (attaquant->nbCompetencesPossedees > 0) {
        for (int i = 0; i < attaquant->nbCompetencesPossedees; i++) {
            printf("%d. Utiliser %s (recharge restant : %d tour(s))\n",
                   i + 2, attaquant->competencesPossedees[i], attaquant->recharge);
        }
    }
    printf("------------------------\n");

    do {
        printf("Votre choix : ");
        scanf("%d", &choix);
    } while (choix < 1 || choix > attaquant->nbCompetencesPossedees + 1);

    int degats = attaquant->attaque - cible->defense;
    if (degats < 0) degats = 0;

    if (attaquant->agilite > rand()%100) { // Vérifie si l'attaquant est plus rapide que la cible
        printf("⚠ %s esquive l'attaque de %s !\n", cible->nom, attaquant->nom);
        pause();
        return;
    }

    if (choix == 1) {
        
        printf("➡ %s de l’équipe %s attaque %s de l’équipe %s\n",
               attaquant->nom, attaquantEquipe->nom, cible->nom, defenseurEquipe->nom);
        cible->pvActuels -= degats;
        if (cible->pvActuels < 0) cible->pvActuels = 0;
        printf("%s inflige %d dégâts à %s (PV restant : %d/%d)\n",
               attaquant->nom, degats, cible->nom, cible->pvActuels, cible->pvMax);
    } else {
        if (attaquant->recharge == 0) {
            char* nomComp = attaquant->competencesPossedees[choix - 2];
            Competence* comp = trouver_competence(competences, nbCompetences, nomComp);
            utiliser_competence(attaquant, cible, comp);
        } else {
            printf("⚠ Compétence en recharge (%d tours restants). Attaque normale.\n", attaquant->recharge);
            printf("➡ %s de l’équipe %s attaque %s de l’équipe %s\n",
                   attaquant->nom, attaquantEquipe->nom, cible->nom, defenseurEquipe->nom);
            cible->pvActuels -= degats;
            if (cible->pvActuels < 0) cible->pvActuels = 0;
            printf("%s inflige %d dégâts à %s (PV restant : %d/%d)\n",
                   attaquant->nom, degats, cible->nom, cible->pvActuels, cible->pvMax);
        }
    }
    pause();  
}

void tour_ia_combat(Combattant* c, Joueur* cEquipe, Joueur* adversaire, int niveau, Competence* competences, int nbCompetences) {
    Combattant* cible = NULL;

    switch (niveau) {
        case 1:  // NOOB : attaque un adversaire aléatoire vivant, pas de compétence
            do {
                cible = &adversaire->equipe[rand() % adversaire->tailleEquipe];
            } while (cible->pvActuels <= 0);

            break;

        case 2:  // FACILE : attaque l'adversaire vivant avec le moins de PV, pas de compétence
            {
                int minPV = 9999;
                for (int i = 0; i < adversaire->tailleEquipe; i++) {
                    if (adversaire->equipe[i].pvActuels > 0 && adversaire->equipe[i].pvActuels < minPV) {
                        minPV = adversaire->equipe[i].pvActuels;
                        cible = &adversaire->equipe[i];
                    }
                }
            }
            break;

        case 3:  // MOYEN : cible le plus faible + utilise une compétence si possible
            {
                int minPV = 9999;
                for (int i = 0; i < adversaire->tailleEquipe; i++) {
                    if (adversaire->equipe[i].pvActuels > 0 && adversaire->equipe[i].pvActuels < minPV) {
                        minPV = adversaire->equipe[i].pvActuels;
                        cible = &adversaire->equipe[i];
                    }
                }

                if (c->recharge == 0 && c->nbCompetencesPossedees > 0) {
                    // Utilise la première compétence disponible
                    char* nomComp = c->competencesPossedees[0];
                    Competence* comp = trouver_competence(competences, nbCompetences, nomComp);
                    utiliser_competence(c, cible, comp);
                    pause();
                    return; // Fin du tour
                }
            }
            break;

        default:
            printf("Niveau IA inconnu !\n");
            return;
    }

    // Si pas de compétence ou niveau 1/2 → attaque normale
    int degats = c->attaque - cible->defense;
    if (degats < 0) degats = 0;

    printf("%s de l’équipe %s attaque %s de l’équipe %s et inflige %d dégâts !\n",
           c->nom, cEquipe->nom, cible->nom, adversaire->nom, degats);

    cible->pvActuels -= degats;
    if (cible->pvActuels < 0) cible->pvActuels = 0;

    pause(); 
}

void demarrer_combat(Joueur* j1, Joueur* j2, Competence* competences, int nbCompetences, int difficulte) {
    for (int i = 0; i < j1->tailleEquipe; i++) j1->equipe[i].barreTour = 0.0f;
    for (int i = 0; i < j2->tailleEquipe; i++) j2->equipe[i].barreTour = 0.0f;

    while (!tous_ko(j1) && !tous_ko(j2)) {
        for (int i = 0; i < j1->tailleEquipe; i++)// Augmentation progressive de la barre de tour en fonction de la vitesse
            if (j1->equipe[i].pvActuels > 0)
                j1->equipe[i].barreTour += j1->equipe[i].vitesse * 0.1f;

        for (int i = 0; i < j2->tailleEquipe; i++)// Même chose 
            if (j2->equipe[i].pvActuels > 0)
                j2->equipe[i].barreTour += j2->equipe[i].vitesse * 0.1f;

        Combattant* actuel = NULL;
        Joueur* joueurActuel = NULL;
        Joueur* adversaire = NULL;

        for (int i = 0; i < j1->tailleEquipe; i++) {// Vérifie si un combattant est prêt à jouer
            if (j1->equipe[i].pvActuels > 0 && j1->equipe[i].barreTour >= 100.0f) {
                actuel = &j1->equipe[i];
                joueurActuel = j1;
                adversaire = j2;
                break;
            }
        }
        if (!actuel) {
            for (int i = 0; i < j2->tailleEquipe; i++) {
                if (j2->equipe[i].pvActuels > 0 && j2->equipe[i].barreTour >= 100.0f) {
                    actuel = &j2->equipe[i];
                    joueurActuel = j2;
                    adversaire = j1;
                    break;
                }
            }
        }

        if (actuel) {
            actuel->barreTour = 0.0f;
            appliquer_effets(actuel);
            mise_a_jour_recharge(actuel);
            lib_place();  

            afficher_equipe(joueurActuel);
            afficher_equipe(adversaire);

            if (joueurActuel == j1 || difficulte == -1) {
                Combattant* cible = choisir_cible(actuel, adversaire, joueurActuel);
                attaquer(actuel, cible, competences, nbCompetences, joueurActuel, adversaire);
            } else {
                tour_ia_combat(actuel, joueurActuel, adversaire, difficulte, competences, nbCompetences);
            }
        }
    }

    lib_place();
    printf("\n\n");
    printf("====================================\n");
    printf("      🏆 COMBAT TERMINÉ ! 🏆\n");
    printf("====================================\n");

    if (tous_ko(j1)) {
        printf("\n🎉🎉 VICTOIRE DE L'ÉQUIPE **%s** ! 🎉🎉\n\n", j2->nom);
    } else {
        printf("\n🎉🎉 VICTOIRE DE L'ÉQUIPE **%s** ! 🎉🎉\n\n", j1->nom);
    }
    printf("====================================\n\n");
    printf("retour au menu principal...\n\n");
    printf("====================================\n\n");
}