#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./model/jeu.h"

#include "./controler/gestion_carte.c"

//#include "./view/GUI.h"
//#include "./view/GUI.c"
//#include "./controler/carte.c"
//#include "./model/controler.h"



int main(int argc, char *argv[])
{

    SDL_Surface *surface;
    SDL_Texture *texture;
    double angle = 0.0;
    SDL_RendererFlip flipType = SDL_FLIP_HORIZONTAL;
    SDL_Rect r = { 10, 10, 50, 50 };
    SDL_Rect clip = { 0, 0, 180, 200 };

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



    surface = IMG_Load("res/tanks.png");
    texture = SDL_CreateTextureFromSurface(renderer,surface);

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
                            r.y -= 15;
                            angle = 0;
                            //deplacer('N', &player, &game);
                        break;
                        case SDLK_RIGHT:
                            r.x += 15;
                            flipType = SDL_FLIP_VERTICAL;
                            angle = 90;
                            //deplacer('E', &player, &game);
                        break;
                        case SDLK_DOWN:
                            r.y += 15;
                            angle = 180;
                            //deplacer('S', &player, &game);
                        break;
                        case SDLK_LEFT:
                            r.x -= 15;
                            flipType = SDL_FLIP_VERTICAL;
                            angle = -90;
                            //deplacer('O', &player, &game);
                        break;
                        case SDLK_SPACE:
                            //shoot();
                        break;
                    };
                    break;
                default: {}
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderPresent(renderer);
        SDL_RenderClear(renderer);


        SDL_RenderFillRect( renderer, &r );
        SDL_SetRenderTarget(renderer, texture);
        SDL_RenderCopyEx(renderer,texture,&clip,&r,angle, NULL, flipType);
        //render_game();
        SDL_SetRenderTarget(renderer, NULL);

        // render things

    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
