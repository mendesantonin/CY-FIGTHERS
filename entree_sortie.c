#include <stdio.h>
#include "entree_sortie.h"


void lib_place() { // Fonction pour nettoyer l'écran
    printf("\033[H\033[J"); 
}

// Fonction pour mettre en pause le programme jusqu'à ce que l'utilisateur appuie sur Entrée
void pause() {
    printf("Appuyez sur Entrée pour continuer...");
    while (getchar() != '\n');
    getchar();

}