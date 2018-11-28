#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>

#include "jeu.h"
#include "gestion_carte.h"

void write_data (game_t * game) {
    FILE *fptr;
    fptr = fopen("res/editeur_map.data", "w");
    fptr = freopen("res/editeur_map.data", "w", fptr);
    if(fptr == NULL) //if file does not exist, create it
    {
        printf("Pas de fichier, création\n");
        fptr = fopen("res/editeur_map.data", "wb");
    }
    int lig, col;
    char c;
    for (lig = 0; lig < HAUTEUR_TAB; lig++) {
        for (col = 0; col < LARGEUR_TAB; col++) {
            c = game->tab_editeur[lig][col];
            fprintf(fptr, "%c", c);
        }
        fprintf(fptr, "%c",'\n' );
    }

    fclose(fptr);
}


void get_case(int lig, int col, game_t * game) {
    if ( (lig >= 0 && col < HAUTEUR_TAB && col >= 0 && lig < LARGEUR_TAB)) {
        if ( (lig < 5 && col < 5) || (lig > 74 && col < 5) ) {
            return;
        }
        game->tab_editeur[col][lig] = game->case_editeur;
        game->tab_editeur[col][ LARGEUR_TAB - lig-1] = game->case_editeur;
    }

}


void afficher_instruction(game_t * game) {
    system("clear");
    char cases[4] = {'M','m','.','B'};
    char * modes[4] = { "Mur incassable", "Mur cassable  ", "Sol           ", "Poussin       " };
    int num = strchr(cases, game->case_editeur)-cases;
    printf("╔══════════════════════════════════════════════════════════════════╗\n");
    printf("║                  COMMANDES EDITEUR DE CARTE                      ║\n");
    printf("╠══════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                  ║\n");
    printf("║   Touche A : Mode mur incassable      (1)                        ║\n");
    printf("║   Touche Z : Mode mur cassable        (2)                        ║\n");
    printf("║   Touche E : Mode sol                 (3)                        ║\n");
    printf("║   Touche R : Mode base                (4)                        ║\n");
    printf("║                                                                  ║\n");
    printf("║   Touche D : Remettre à zéro la carte                            ║\n");
    printf("║   Touche S : Sauvegarde de la map                                ║\n");
    printf("║                                                                  ║\n");
    printf("╠══════════════════════════════════════════════════════════════════╣\n");
    printf("║                                                                  ║\n");
    printf("║      Mode actuel :    %s     (%d)                     ║\n", modes[num], num+1);
    printf("║                                                                  ║\n");
    printf("╚══════════════════════════════════════════════════════════════════╝\n");
}
