#include <stdlib.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "./model/jeu.h"
//#include "./view/GUI.h"
#include "./view/GUI.c"
#include "./controler/carte.c"
//#include "./model/controler.h"



int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "Could not initialize sdl2: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window *window = SDL_CreateWindow("Tanks Battle", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);

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

    joueur_t player = {
        .posLig = 42,
        .posCol = 48,
        .oldPosLig = -1,
        .oldPosCol = -1
    };

    game_t game = {
        .tab = create_tab(WINDOW_HEIGHT/TAILLE, WINDOW_WIDTH/TAILLE),
        .state = EN_COURS,
    };

    tank_update(&game, &player, 'X');

    const float cell_width = WINDOW_WIDTH / TAILLE;

    SDL_Event e;
    while (game.state != FIN_JEU) {
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    game.state = FIN_JEU;
                break;

                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_UP)
                        deplacer('N', &player, &game);
                    else if(e.key.keysym.sym == SDLK_RIGHT)
                        deplacer('E', &player, &game);
                    else if(e.key.keysym.sym == SDLK_DOWN)
                        deplacer('S', &player, &game);
                    else if(e.key.keysym.sym == SDLK_LEFT)
                        deplacer('O', &player, &game);
                    break;

                default: {}
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        render_game(renderer, &game);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
