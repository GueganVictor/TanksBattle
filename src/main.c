#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

#include "jeu.h"
#include "gestion_editeur.h"
#include "gestion_jeu.h"
#include "GUI.h"
#include "gestion_carte.h"
#include "gestion_listes.h"

#include "dev_tools.h"
#include "gestion_clavier.h"


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
        .pos_lig = 40,
        .pos_col = 25,
        .blindage = 3,

        .type = 'J',
        .etat = EN_VIE,

        .blindage_orig = 3,

        .nxt = NULL
    };

    game_t game = {
        .tab = create_tab(HAUTEUR_TAB, LARGEUR_TAB, "res/map.data"),//create_tab(HAUTEUR_TAB, LARGEUR_TAB),
        .etat = EN_MENU,
        .tab_editeur = create_tab(HAUTEUR_TAB, LARGEUR_TAB, "res/editeur_map.data"),
        .case_editeur = 'm',
        .difficulte = NON_DEFINI,
        .choix_menu = 0,
        .tanks_tue = 0,
        .tail = &joueur,
        .head = &joueur
    };

    tank_update(&game, &joueur, 'T');

    if (!init_sons (&game) ) {
        printf("Erreur dans le chargement des fichiers sons !\n");
        return EXIT_FAILURE;
    }

    printf("Volume : %d\n", Mix_VolumeMusic(MIX_MAX_VOLUME/6) );
    printf("Volume : %d\n", Mix_VolumeChunk(game.effets[0], MIX_MAX_VOLUME/6) );
    printf("Volume : %d\n", Mix_VolumeChunk(game.effets[1], MIX_MAX_VOLUME/6) );

    obus_t obus = {
        .num_obus = -1
    };

    tank_update(&game, &joueur, 'X');

    init_image(renderer, &game);

    int temps_tir_joueur = 0;
    game.temps_tick = 0;
    int temps_ajout_tank = 0;
    int temps_tir_enemi = 0;
    int temps_change_etat = 0;
    int temps_deplacement = 0;
    int temps_deplacement_obus = 0;
    int temps_maj_obus = 0;
    int temps_render = 0;
    int temps_bonus = 0;
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
                                valider_choix_menu(&game, &joueur, &obus);
                            break;
                            case SDL_KEYDOWN:
                                changement_touche_menu(&game, e.key.keysym.sym, &joueur, &obus);
                            break;
                        }
                    break;
                    case GAME_OVER :
                        changement_touche_fin_jeu(&game, e.key.keysym.sym, &joueur, &obus);
                    break;
                    case GAME_WON :
                        changement_touche_fin_jeu(&game, e.key.keysym.sym, &joueur, &obus);
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
                if (SDL_GetTicks() >= temps_deplacement + (10-(game.difficulte))*TICKRATE ) {
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
                if (SDL_GetTicks() >= temps_tir_enemi + (200-(game.difficulte*40))*TICKRATE ) {
                    tirer_enemi(&joueur, &game, &obus);
                    temps_tir_enemi = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps_ajout_tank + (350-(game.difficulte*75))*TICKRATE) {
                    ajouter_tank(&joueur, &game);
                    temps_ajout_tank = SDL_GetTicks();
                }
                if (SDL_GetTicks() >= temps_bonus + 1000*TICKRATE) {
                    ajouter_bonus(&game);
                    temps_bonus = SDL_GetTicks();
                }
                verif_victoire (&game);
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
    Mix_FreeChunk(game.effets[0]);
    Mix_FreeChunk(game.effets[1]);
    Mix_CloseAudio();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
