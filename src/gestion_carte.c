#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "jeu.h"
#include "gestion_listes.h"

// alloue dynamiquement la mémoire pour le tableau
char ** alloc_tab (int lig, int col) {
    char** array = malloc(lig*sizeof(char*));
    for (int i = 0; i < lig; i++) {
        array[i] = malloc(col*sizeof(char));
    }
    return array;
}

// met à jour la position d'un tank sur la carte
void tank_update( game_t *game, tank_t *tank, char car) {
    int posLig = tank->pos_lig;
    int posCol = tank->pos_col;
    game->tab[posLig-1][posCol-1] = car;
    game->tab[posLig][posCol] = car;
    game->tab[posLig+1][posCol] = car;
    game->tab[posLig][posCol+1] = car;
    game->tab[posLig][posCol-1] = car;
    game->tab[posLig-1][posCol] = car;
    game->tab[posLig+1][posCol-1] = car;
    game->tab[posLig+1][posCol+1] = car;
    game->tab[posLig-1][posCol+1] = car;
}

// fonction pour afficher la carte sur le terminal
void show_tab_term (int nbLig, int nbCol, char ** tab) {
    for (int lig = 0; lig < nbLig; lig++) {
        for (int col = 0; col < nbCol; col++) {
            printf("%c", tab[lig][col]);
        }
        printf("  -  lig %d\n", lig);
    }
    printf("\n");
}

// vérifie si un tank peut avancer
int verif_deplacement ( game_t * game, tank_t * tank , int lig, int col, int lig1, int col1, int lig2, int col2 ) {
    if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'A' ) {

        if (game->tab[lig][col] == 'A' && tank->type == 'J') appliquer_bonus(game);

        if (game->tab[lig1][col1] == '.' || game->tab[lig1][col1] == 'A' ) {

            if (game->tab[lig1][col1] == 'A' && tank->type == 'J') appliquer_bonus(game);

            if (game->tab[lig2][col2] == '.' || game->tab[lig2][col2] == 'A' ) {

                if (game->tab[lig2][col2] == 'A' && tank->type == 'J') appliquer_bonus(game);

                return 1;
            }
        }
    }
    return 0;
}

// rempli le tableau avec les caractères du fichier placés en paramètre
void remplissage_tab(int nbLig, int nbCol, char** tab, char * fichier) {
    char c;
    FILE *file;
    file = fopen(fichier, "r");
    if (file) {
        printf("Fichier chargé.\n");
        for (int lig = 0; lig < nbLig; lig++) {
            for (int col = 0; col < nbCol; col++) {
                if ((c = getc(file)) != EOF) {
                    if ( c > 31 && c < 123 ){
                        tab[lig][col] = c;
                    } else {
                        col--;
                    }
                }
            }
        }
        fclose(file);
    }
}

// créer le tableau de la carte
char ** create_tab(int nbLig, int nbCol, char * fichier) {
    printf("Taille carte : %d x %d\n", nbLig, nbCol );
    char **tab = alloc_tab(nbLig, nbCol);
    remplissage_tab(nbLig, nbCol, tab, fichier);
    return tab;
}

// rempli un tableau vide
void remplissage_tab_vide(int nbLig, int nbCol, char** tab) {
    for (int lig = 0; lig < nbLig; lig++) {
        for (int col = 0; col < nbCol; col++) {
            tab[lig][col] = '.';
        }
    }
}
// renvoit un tableau vide
char ** create_tab_vide(int nbLig, int nbCol) {
    char **tab = alloc_tab(nbLig, nbCol);
    remplissage_tab_vide(nbLig, nbCol, tab);
    return tab;
}
