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

void init_image (SDL_Renderer * renderer, game_t * game) {

    SDL_Surface * surface;

    surface = IMG_Load("res/Tile_Map.png");
    game->textures[0] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TankMap.png");
    game->textures[1] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Menu.png");
    game->textures[2] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Overlay.png");
    game->textures[3] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TankGlow.png");
    game->textures[4] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Neon_Sol.png");
    game->textures[5] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/MenuSelector.png");
    game->textures[6] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Overlay_Item_Map.png");
    game->textures[7] = SDL_CreateTextureFromSurface(renderer,surface);

    SDL_FreeSurface( surface );

}

int init_sons (game_t * game) {
    game->music = Mix_LoadMUS("res/theme.mp3");
    game->effets[0] = Mix_LoadWAV("res/button.wav");
    game->effets[1] = Mix_LoadWAV("res/shoot.wav");


    if ( game->effets[0] == NULL || game->effets[1] == NULL || game->music == NULL) {
        return 0;
    }
    return 1;

}

void reset_game (game_t * game, tank_t * liste_tank, obus_t * liste_obus) {
    printf("Remise a zéro du jeu.\n");
    if (liste_obus->nxt != NULL) {
        free_liste_obus(liste_obus->nxt, game);
    }

    if (liste_tank->nxt != NULL) {
        free_liste_tank(liste_tank->nxt, game);
    }

    liste_tank->num_tank = 0;
    liste_tank->direction = 'N';
    liste_tank->pos_lig = 15;
    liste_tank->pos_col = 1;
    liste_tank->blindage = 3;

    liste_tank->type = 'J';
    liste_tank->etat = EN_VIE;

    liste_tank->blindage_orig = 3;
    liste_tank->nb_hit = 0;

    liste_tank->nxt = NULL;

    game->tab = create_tab(HAUTEUR_TAB, LARGEUR_TAB, "res/map.data");
    game->etat = EN_MENU;
    game->tab_editeur = create_tab(HAUTEUR_TAB, LARGEUR_TAB, "res/editeur_map.data");
    game->case_editeur = 'm';
    game->difficulte = NON_DEFINI;
    game->choix_menu = 0;
    game->tanks_tue = 0;
    game->tail = liste_tank;

    liste_obus->num_obus = -1;
    liste_obus->nxt = NULL;

}

void verif_victoire (game_t * game) {
    if (NB_TANKS - game->tanks_tue < 1) {
        printf("Partie finie, gagné !");
        game->etat = GAME_WON;
    }
}

void application_difficulte(game_t * game, int difficulte, tank_t * liste_tank, obus_t * liste_obus) {

    switch (game->difficulte) {
        case FACILE:
            if (difficulte != FACILE) {
                reset_game(game, liste_tank, liste_obus);
                game->tank_restant[0] = 15;
                game->tank_restant[1] = 12;
                game->tank_restant[2] =  3;
                game->difficulte = FACILE;
                game->etat = EN_JEU;
            }
        break;
        case DIFFICILE:
            if (difficulte != DIFFICILE) {
                reset_game(game, liste_tank, liste_obus);
                game->tank_restant[0] = 5;
                game->tank_restant[1] = 10;
                game->tank_restant[2] = 15;
                game->difficulte = DIFFICILE;
                game->etat = EN_JEU;
            }
        break;
        case INFERNO:
            if (difficulte != INFERNO) {
                reset_game(game, liste_tank, liste_obus);
                game->tank_restant[0] = 0;
                game->tank_restant[1] = 10;
                game->tank_restant[2] = 20;
                game->difficulte = INFERNO;
                game->etat = EN_JEU;
            }
        break;
        case NON_DEFINI:
            printf("yo\n");
            switch (difficulte) {
                case FACILE:
                    game->tank_restant[0] = 15;
                    game->tank_restant[1] = 12;
                    game->tank_restant[2] =  3;
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
