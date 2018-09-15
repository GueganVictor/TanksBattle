#include <math.h>
#include <SDL2/SDL.h>

//#include "./GUI.h"

const SDL_Color WALL_COLOR = { .r = 0, .g = 0, .b = 0 };
const SDL_Color SPACE_COLOR = { .r = 255, .g = 255, .b = 255 };

void render_game(SDL_Renderer *renderer, const game_t *game) {
    int posX = 0;
    int posY = 0;
    for (size_t lig = 0; lig < WINDOW_HEIGHT/TAILLE; lig++) {
        for (size_t col = 0; col < WINDOW_WIDTH/TAILLE; col++) {
            SDL_Rect rect;
            rect.x = posX;
            rect.y = posY;
            rect.w = TAILLE;
            rect.h = TAILLE;
            switch (game->tab[lig][col]) {
                case ' ':
                    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255);
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'M':
                    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'P':
                    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'm':
                    SDL_SetRenderDrawColor( renderer, 255, 100, 100, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                default: {}
            }
            posX = posX + TAILLE;
        }
        posX = 0;
        posY = posY + TAILLE;
    }

}
