#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "./model/jeu.h"

#include "./view/GUI.c"

#include "./controler/gestion_carte.c"
#include "./controler/gestion_tanks.c"
#include "./controler/gestion_obus.c"


//#include "./view/GUI.h"
//#include "./view/GUI.c"
//#include "./controler/carte.c"
//#include "./model/controler.h"



int main(int argc, char *argv[])
{

    srand(time(NULL));


    char * directions = "NOSE";

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Erreur d'initialisation de la sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    if(TTF_Init() == -1) {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Window *window = SDL_CreateWindow("Tanks Battle", 100, 100, LARGEUR_FENTRE, HAUTEUR_FENTRE, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "SDL_CreateWindow Error : %s", SDL_GetError() );
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error : %s", SDL_GetError() );
        return EXIT_FAILURE;
    }

    game_t game = {
        .tab = create_tab(HAUTEUR_FENTRE/TAILLE, LARGEUR_FENTRE/TAILLE),//create_tab(HAUTEUR_FENTRE/TAILLE, LARGEUR_FENTRE/TAILLE),
        .etat = EN_MENU
    };

    tank_t joueur = {
        .num_tank = 0,
        .direction = 'E',
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
    int bOk = 1;

    tank_update(&game, &joueur, 'X');

    surface = IMG_Load("res/TileMap.png");
    tilemap_sol = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TanksMap.png");
    tanks = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/logo2.png");
    logo = SDL_CreateTextureFromSurface(renderer,surface);
    police = TTF_OpenFont("res/roboto.ttf", 121);

    ajouter_tank(&joueur, &game);
    ajouter_tank(&joueur, &game);

    //ajouter_tank(&joueur, &game);



    int cpt = 0;

    SDL_Event e;
    while (game.etat != FIN_JEU) {
        while (SDL_PollEvent(&e)) {
            if (game.etat == EN_JEU) {
                switch (e.type) {
                    case SDL_QUIT:
                        game.etat = FIN_JEU;
                    break;
                    case SDL_KEYDOWN:
                        switch (e.key.keysym.sym) {
                            case SDLK_UP:
                                joueur.direction = 'N';
                                deplacer(&joueur, &game);
                            break;
                            case SDLK_RIGHT:
                                joueur.direction = 'E';
                                deplacer(&joueur, &game);
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
                                ajouter_obus(&joueur, &game,&obus);
                            break;
                        };

                        break;
                    default: {}
                }
            }
            if (game.etat == EN_MENU) {
                switch (e.type) {
                    case SDL_QUIT:
                        game.etat = FIN_JEU;
                    break;
                    case SDL_KEYDOWN:
                        switch (e.key.keysym.sym) {
                            case SDLK_SPACE:
                                game.etat = EN_JEU;
                                printf("Passage en mode JEU (1) %d\n", game.etat );
                            break;
                        }
                }
            }
        }
            //supprimerTank(&joueur, 1);
            //if (cpt == 2000 && bOk == 1) {
        if (game.etat == EN_JEU && (SDL_GetTicks() % 200 == 0)) {
            afficherobus(&obus);
            deplacer_tanks(&joueur, &game);
            //printf("avant depalcer obus\n" );
            deplacer_obus(&joueur, &game, &obus);
        }
            //bOk = 0;
            //ajouter_tank(&joueur, &game);
        //}

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);

        switch (game.etat) {
            case EN_JEU:
                cpt++;
                render_game(renderer, &game, &joueur);
            break;
            case EN_MENU:
                render_menu(renderer, &game);
            break;
            case EDITEUR:
                //render_editeur(renderer, &game);
            break;
        }

    }

    SDL_FreeSurface( surface );
    TTF_CloseFont( police );

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
