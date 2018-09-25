#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./model/jeu.h"

#include "./view/GUI.c"

#include "./controler/gestion_carte.c"
#include "./controler/gestion_liste.c"

//#include "./view/GUI.h"
//#include "./view/GUI.c"
//#include "./controler/carte.c"
//#include "./model/controler.h"



int main(int argc, char *argv[])
{

    srand(time(NULL));

    SDL_Rect r = { 10, 10, 50, 50 };
    SDL_Rect clip = { 0, 0, 180, 200 };

    char * directions = "NOSE";

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
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
        .state = EN_COURS,
    };

    tank_t joueur = {
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




    tank_update(&game, &joueur, 'X');

    surface = IMG_Load("res/TileMap.png");
    tilemap_sol = SDL_CreateTextureFromSurface(renderer,surface);

    surface = IMG_Load("res/TanksMap.png");
    tanks = SDL_CreateTextureFromSurface(renderer,surface);

    //ajouter_tank(&joueur, &game);

    //ajouter_tank(&joueur, &game);



    int cpt = 0;

    SDL_Event e;
    while (game.state != FIN_JEU) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    game.state = FIN_JEU;
                break;

                case SDL_KEYDOWN:
                    switch (e.key.keysym.sym) {
                        case SDLK_UP:

                            joueur.direction = 'N';
                            //deplacer('N', &player, &game);
                        break;
                        case SDLK_RIGHT:
                            //flipType = SDL_FLIP_VERTICAL;

                            joueur.direction = 'E';
                            //deplacer('E', &player, &game);
                        break;
                        case SDLK_DOWN:

                            joueur.direction = 'S';
                            //deplacer('S', &player, &game);
                        break;
                        case SDLK_LEFT:

                            joueur.direction = 'O';
                            //flipType = SDL_FLIP_VERTICAL;
                            //deplacer('O', &player, &game);
                        break;
                        case SDLK_SPACE:
                            //shoot();
                        break;
                    };
                    deplacer(&joueur, &game);
                    break;
                default: {}
            }
        }

        cpt++;
        if (cpt == 500) {
            //deplacer_tanks(&joueur, &game);

            cpt = 0;
        }


        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);
        render_game(renderer, &game, &joueur);
        // render things


    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
