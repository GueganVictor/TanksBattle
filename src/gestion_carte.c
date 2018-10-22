#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "jeu.h"

char ** alloc_tab (int lig, int col) {
    char** array = malloc(lig*sizeof(char*));
    for (int i = 0; i < lig; i++) {
        array[i] = malloc(col*sizeof(char));
    }
    return array;
}

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

void show_tab_term (int nbLig, int nbCol, char ** tab) {
    for (int lig = 0; lig < nbLig; lig++) {
        for (int col = 0; col < nbCol; col++) {
            printf("%c", tab[lig][col]);
        }
        printf("  -  lig %d\n", lig);
    }
    printf("\n");
}


void deplacer(tank_t *tank, game_t *game ) {
    tank_update(game, tank, '.');

    int col, lig;
    switch (tank->direction) {
        case 'N':
            if (tank->pos_lig == 1) {
                break;
            } else {
                lig = tank->pos_lig-2;
                col = tank->pos_col-1;
                if (game->tab[lig][col] == '.' && game->tab[lig][col+1] == '.' && game->tab[lig][col+2] == '.') {
                    tank->pos_lig = tank->pos_lig-1;
                }
            }

        break;
        case 'O':
            if (tank->pos_col == 1) {
                break;
            } else {
                lig = tank->pos_lig-1;
                col = tank->pos_col-2;
                if (game->tab[lig][col] == '.' && game->tab[lig+1][col] == '.' && game->tab[lig+2][col] == '.') {
                    tank->pos_col = tank->pos_col-1;
                }
            }
        break;
        case 'S':
            if (tank->pos_lig == HAUTEUR_TAB-2) {
                break;
            } else {
                lig = tank->pos_lig+2;
                col = tank->pos_col-1;
                if (game->tab[lig][col] == '.' && game->tab[lig][col+1] == '.' && game->tab[lig][col+2] == '.') {
                    tank->pos_lig = tank->pos_lig+1;
                }
            }
        break;
        case 'E':
            if (tank->pos_col == LARGEUR_TAB-2) {
                break;
            } else {
                lig = tank->pos_lig-1;
                col = tank->pos_col+2;
                if (game->tab[lig][col] == '.' && game->tab[lig+1][col] == '.' && game->tab[lig+2][col] == '.') {
                    tank->pos_col = tank->pos_col+1;
                }
            }
        break;
    }
    tank_update(game, tank, 'X');
    if (tank->type == 'E') {
        tank_update(game, tank, 'E');
    }

    //show_tab_term(HAUTEUR_TAB, LARGEUR_TAB, game->tab);
}

int verif_deplacement (tank_t * tank, game_t * game) {
    return 1;
}

void remplissage_tab(int nbLig, int nbCol, char** tab) {
    char * fichier = "res/editeur_map.data";
    char c;
    FILE *file;
    file = fopen(fichier, "r");
    printf("remplissage carte : %d x %d\n", nbLig, nbCol );
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


char ** create_tab(int nbLig, int nbCol) {
    printf("Taille carte : %d x %d\n", nbLig, nbCol );
    char **tab = alloc_tab(nbLig, nbCol);
    remplissage_tab(nbLig, nbCol, tab);
    return tab;
}


void remplissage_tab_vide(int nbLig, int nbCol, char** tab) {
    for (int lig = 0; lig < nbLig; lig++) {
        for (int col = 0; col < nbCol; col++) {
            tab[lig][col] = '.';
        }
    }
}

char ** create_tab_vide(int nbLig, int nbCol) {
    char **tab = alloc_tab(nbLig, nbCol);
    remplissage_tab_vide(nbLig, nbCol, tab);
    return tab;
}
