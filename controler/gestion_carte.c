#include <stdio.h>
#include <stdlib.h>

char ** alloc_tab (int lig, int col) {
    char** array = malloc(lig*sizeof(char*));
    for (int i = 0; i < lig; i++) {
        array[i] = malloc(col*sizeof(char));
    }
    return array;
}

void deplacer(char dir, joueur_t *joueur, game_t *game ) {
    joueur->oldPosCol = joueur->posCol;
    joueur->oldPosLig = joueur->posLig;
    tank_update(game, joueur, ' ');
    switch (dir) {
        case 'N':
            joueur->posLig = joueur->posLig-1;
        break;
        case 'O':
            joueur->posCol = joueur->posCol-1;
        break;
        case 'S':
            joueur->posLig = joueur->posLig+1;
        break;
        case 'E':
            joueur->posCol = joueur->posCol+1;
        break;
    }

    tank_update(game, joueur, 'X');
}

void remplissage_tab(int nbLig, int nbCol, char** tab) {
    char * fichier = "res/map.data";
    int c;
    FILE *file;
    file = fopen(fichier, "r");
    printf("remplissage carte : %d x %d\n", nbLig, nbCol );
    if (file) {
        printf("Fichier chargé.\n");
        for (int lig = 0; lig < nbLig; lig++) {
            for (int col = 0; col < nbCol; col++) {
                if (((c = getc(file)) != EOF)) {
                    if ( verif_char(c)) {
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

void show_tab_term (int nbLig, int nbCol, char ** tab) {

    for (int lig = 0; lig < nbLig; lig++) {
        for (int col = 0; col < nbCol; col++) {
            printf("%c-", tab[lig][col]);
        }
        printf("  -  lig %d\n", lig);
    }
    printf("\n");
}

char ** create_tab(int nbLig, int nbCol) {
    printf("Taille carte : %d x %d\n", nbLig, nbCol );
    //int **tab = alloc_tab(nbLig, nbCol);
    char **tab = alloc_tab(nbLig, nbCol);
    remplissage_tab(nbLig, nbCol, tab);
    //show_tab_term(nbLig, nbCol, tab);
    return tab;

}
