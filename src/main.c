#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "jeu.h"
#include "gestion_editeur.h"
#include "GUI.h"

#include "gestion_carte.h"
#include "gestion_listes.h"
#include "dev_tools.h"

/*
faut compiler avec -g
et si t'as une erreur de segmentation
tu fait :
gdb
gdb nomdelexec
*/

SDL_Renderer * initialisation_SDL(SDL_Window * window) {
    srand(time(NULL));

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation de la sdl2: %s\n", SDL_GetError());
        return NULL;
    }

    if(TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
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

    return renderer;
}

int main(int argc, char *argv[])
{
    SDL_Window * window = NULL;
    SDL_Renderer * renderer = initialisation_SDL(window);

    if (renderer == NULL) {
        return EXIT_FAILURE;
    }

    game_t game = {
        .tab = create_tab(HAUTEUR_TAB, LARGEUR_TAB),//create_tab(HAUTEUR_TAB, LARGEUR_TAB),
        .etat = EN_MENU,
        .tab_editeur = create_tab_vide(HAUTEUR_TAB, LARGEUR_TAB),
        .case_editeur = 'M'
    };

    tank_t joueur = {
        .num_tank = 0,
        .direction = 'N',
        .pos_lig = 42,
        .pos_col = 49,
        .blindage = 2,

        .type = 'J',
        .etat = 1,

        .blindage_orig = 2,
        .nb_hit = 0,

        .nxt = NULL
    };

    obus_t obus = {
        .num_obus = -1
    };

    tank_update(&game, &joueur, 'X');

    SDL_Surface * surface;

    surface = IMG_Load("res/TileMap.png");
    game.textures[0] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TanksMap.png");
    game.textures[1] = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/logo2.png");
    game.textures[2] = SDL_CreateTextureFromSurface(renderer,surface);

    int dernier_temps = 0;
    SDL_Event e;
    while (game.etat != FIN_JEU) {
            while (SDL_PollEvent(&e)) {
                if (e.type == SDL_QUIT) {
                        game.etat = FIN_JEU;
                }
                switch (game.etat) {
                    case EDITEUR :
                        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN ) {
                            if (SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                                get_case(e.button.x/TAILLE, e.button.y/TAILLE, &game);
                            }
                        }
                        if (e.type == SDL_KEYDOWN ) {
                            changement_mode(&game, e.key.keysym.sym);
                        }

                    break;
                    case EN_JEU :
                        switch (e.type) {
                            case SDL_KEYDOWN:
                                switch (e.key.keysym.sym) {
                                    case SDLK_UP:
                                        joueur.direction = 'N';
                                        deplacer(&joueur, &game);
                                    break;
                                    case SDLK_g:
                                        afficher_instruction(&game);
                                    break;
                                    case SDLK_RIGHT:
                                        joueur.direction = 'E';
                                        deplacer(&joueur, &game);
                                    break;
                                    case SDLK_a:
                                        ajouter_tank(&joueur, &game);
                                    break;
                                    case SDLK_DOWN:
                                        joueur.direction = 'S';
                                        deplacer(&joueur, &game);
                                    break;
                                    case SDLK_LEFT:
                                        joueur.direction = 'O';
                                        deplacer(&joueur, &game);
                                    break;
                                    case SDLK_SPACE:
                                        if (SDL_GetTicks() > dernier_temps + 250 ) {
                                            tirer_obus(&joueur, &game,&obus);
                                            dernier_temps = SDL_GetTicks();
                                        }
                                    break;
                                    case SDLK_ESCAPE:
                                        game.etat = EN_MENU;
                                    break;
                                    case SDLK_o:
                                        print_list_obus(&obus);
                                    break;
                                    case SDLK_p:
                                        print_list_tank(&joueur);
                                    break;
                                    case SDLK_l:
                                        decrease_armor(&joueur);
                                    break;
                                    default:
                                    break;
                                };

                                break;
                            default: {}
                        }
                    break;
                    case EN_MENU :
                        switch (e.type) {
                            case SDL_KEYDOWN:
                                switch (e.key.keysym.sym) {
                                    case SDLK_SPACE:
                                        game.etat = EN_JEU;
                                        printf("Passage en mode JEU (1) %d\n", game.etat );
                                    break;
                                    case SDLK_e:
                                        game.etat = EDITEUR;
                                        afficher_instruction(&game);
                                        printf("Passage en mode EDITOR (2) %d\n", game.etat );
                                    break;
                                }
                        }
                    break;
                }
            }

        if (game.etat == EN_JEU && (SDL_GetTicks() % 150 == 0)) {
            deplacer_tanks(&joueur, &game);
        }
        if (game.etat == EN_JEU && (SDL_GetTicks() % 50 == 0)) {
            deplacer_obus(&joueur, &game, &obus);
        }

        /*if (game.etat == EN_JEU && (SDL_GetTicks() % 4000 == 0 )) {
            printf("tirer enemi\n");
            tirer_enemi(&joueur, &game, &obus);
        }*/

        if ((SDL_GetTicks() % 10 == 0)) {

            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderPresent(renderer);
            SDL_RenderClear(renderer);

            switch (game.etat) {
                case EN_JEU:
                    render_game(renderer, &game, &joueur, &obus);
                break;
                case EN_MENU:
                    render_menu(renderer, &game);
                break;
                case EDITEUR:
                    render_editeur(renderer, &game);
                break;
            }
        }
    }
    SDL_FreeSurface( surface );

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
