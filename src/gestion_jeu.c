#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "jeu.h"
#include "gestion_carte.h"
#include "gestion_editeur.h"
#include "gestion_listes.h"
#include "dev_tools.h"


void reset_game () {
    printf("Remise a zéro du jeu.\n");
}

void application_difficulte(game_t * game, int difficulte) {

    switch (game->difficulte) {
        case FACILE:
            if (difficulte != FACILE) {
                reset_game();
                game->tank_restant[0] = 17;
                game->tank_restant[1] = 18;
                game->tank_restant[2] =  5;
                game->difficulte = FACILE;
            }
        break;
        case DIFFICILE:
            if (difficulte != DIFFICILE) {
                reset_game();
                game->tank_restant[0] = 5;
                game->tank_restant[1] = 10;
                game->tank_restant[2] = 15;
                game->difficulte = DIFFICILE;
            }
        break;
        case NON_DEFINI:
            printf("yo\n");
            switch (difficulte) {
                case FACILE:
                    game->tank_restant[0] = 17;
                    game->tank_restant[1] = 18;
                    game->tank_restant[2] =  5;
                    game->difficulte = FACILE;
                break;
                case DIFFICILE:
                    game->tank_restant[0] = 5;
                    game->tank_restant[1] = 10;
                    game->tank_restant[2] = 15;
                    game->difficulte = DIFFICILE;
                break;
            }
        break;
    }


}
