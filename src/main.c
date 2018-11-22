#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "jeu.h"
#include "gestion_editeur.h"
#include "GUI.h"
#include "gestion_carte.h"
#include "gestion_listes.h"

#include "dev_tools.h"
#include "gestion_clavier.h"

/*
faut compiler avec -g
et si t'as une erreur de segmentation
tu fait :
gdb
gdb nomdelexec
*/

SDL_Renderer * initialisation_SDL(SDL_Window * window) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "Erreur d'initialisation de la sdl2: %s\n", SDL_GetError());
        return NULL;
    }

    window = SDL_CreateWindow("Tanks Battle", 100, 100, LARGEUR_FENETRE, HAUTEUR_FENETRE, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error : %s", SDL_GetError() );
        return NULL;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error : %s", SDL_GetError() );
        return NULL;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024 ) == -1 )
		return NULL;

    return renderer;
}


int main(int argc, char *argv[])
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = initialisation_SDL(window);

    tank_t joueur = {
        .num_tank = 0,
        .direction = 'N',
        .pos_lig = 15,
        .pos_col = 1,
        .blindage = 2,

        .type = 'J',
        .etat = EN_VIE,

        .blindage_orig = 2,
        .nb_hit = 0,

        .nxt = NULL
    };

    game_t game = {
        .tab = create_tab(HAUTEUR_TAB, LARGEUR_TAB),//create_tab(HAUTEUR_TAB, LARGEUR_TAB),
        .etat = EN_MENU,
        .tab_editeur = create_tab_vide(HAUTEUR_TAB, LARGEUR_TAB),
        .case_editeur = 'M',
        .difficulte = NON_DEFINI,
        .explosion = Mix_LoadWAV("res/swoosh.wav"),
        .music = Mix_LoadMUS("res/battlefield.mp3"),
        .choix_menu = 0,
        .tail = &joueur
    };

    if ( game.explosion == NULL || game.music == NULL) {
        printf("Erreur dans le chargement des fichiers sons !\n");
        return EXIT_FAILURE;
    }

    printf("Volume : %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME/6) );

    obus_t obus = {
        .num_obus = -1
    };

    tank_update(&game, &joueur, 'X');

    SDL_Surface * surface;

    surface = IMG_Load("res/Tile_Map.png");
    game.textures[0] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TankMap.png");
    game.textures[1] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Menu.png");
    game.textures[2] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Overlay.png");
    game.textures[3] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TankGlow.png");
    game.textures[4] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Neon_Sol.png");
    game.textures[5] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/MenuSelector.png");
    game.textures[6] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/Overlay_Item_Map.png");
    game.textures[7] = SDL_CreateTextureFromSurface(renderer,surface);


    int temps_tir_joueur = 0;
    game.temps_tick = 0;
    int temps = 0;
    int temps_tir_enemi = 0;
    int temps_change_etat = 0;
    int temps_deplacement = 0;
    int temps_deplacement_obus = 0;
    int temps_maj_obus = 0;
    int temps_render = 0;
    SDL_Event e;
    game.cpt = 0;
    while (game.etat != FIN_JEU) {
            Mix_PlayingMusic();
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT)
                        game.etat = FIN_JEU;
                switch (game.etat) {
                    case EDITEUR :
                        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN )
                            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT))
                                get_case((e.button.x-(18*TAILLE))/TAILLE, (e.button.y-(2*TAILLE))/TAILLE, &game);
                        if (e.type == SDL_KEYDOWN )
                            changement_mode_editeur(&game, e.key.keysym.sym);
                    break;
                    case EN_JEU :
                        if (e.type == SDL_KEYDOWN)
                            changement_touche_jeu(&game, &joueur, &obus, e.key.keysym.sym, &temps_tir_joueur);
                    break;
                    case EN_MENU :
                        switch (e.type) {
                            case SDL_MOUSEMOTION:
                                deplacement_souris_menu(&game, e.button.x, e.button.y);
                            break;
                            case SDL_MOUSEBUTTONDOWN:
                                valider_choix_menu(&game);
                            break;
                            case SDL_KEYDOWN:
                                changement_touche_menu(&game, e.key.keysym.sym);
                            break;
                        }
                    break;
                    case GAME_OVER :
                        changement_touche_fin_jeu(&game, e.key.keysym.sym);
                    break;
                    case GAME_WON :
                        changement_touche_fin_jeu(&game, e.key.keysym.sym);
                    break;
                }
            }


        //print_tps(&game);

        switch (game.etat) {
            case EN_JEU:
                if (SDL_GetTicks() >= temps_change_etat + 10*TICKRATE ) {
                    change_etat_tank(&joueur, &game);
                    temps_change_etat = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps_deplacement + 10*TICKRATE ) {
                    deplacer_tanks(&joueur, &game);
                    temps_deplacement = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps_deplacement_obus + 4*TICKRATE ) {
                    deplacer_obus(&joueur, &game, &obus);
                    temps_deplacement_obus = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps_maj_obus + (10*TICKRATE)/4 ) {
                    maj_obus(&obus, &game);
                    temps_maj_obus = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps_tir_enemi + (150)*TICKRATE ) {
                    tirer_enemi(&joueur, &game, &obus);
                    temps_tir_enemi = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps + 250*TICKRATE) {
                    ajouter_tank(&joueur, &game);
                    temps = SDL_GetTicks();
                }
            break;
            case EN_MENU:
            break;
            case EDITEUR:
            break;
        }
        if (SDL_GetTicks() > temps_render + 10) {
            game.cpt++;
            SDL_RenderClear(renderer);
            refresh_screen(renderer, &game, &joueur, &obus);
            temps_render = SDL_GetTicks();
        }
    }
    Mix_FreeMusic(game.music);
    Mix_FreeChunk(game.explosion);
    Mix_CloseAudio();
    SDL_FreeSurface( surface );
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
