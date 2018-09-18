#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
//#include "./GUI.h"

const SDL_Color WALL_COLOR = { .r = 0, .g = 0, .b = 0 };
const SDL_Color SPACE_COLOR = { .r = 255, .g = 255, .b = 255 };

void afficher_tank(SDL_Renderer *renderer, const tank_t *joueur, SDL_Rect rect, SDL_Rect * clipa) {




}


void render_game(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur) {

    SDL_Rect textu;
    textu.x = 0;
    textu.y = 0;
    textu.w = 10;
    textu.h = 10;

    SDL_Rect nord = { 48, 0, 16, 16};
    SDL_Rect ouest = { 96, 0, 16, 16};
    SDL_Rect est = { 144, 0, 16, 16};
    SDL_Rect sud = { 0, 0, 16, 16};

    SDL_Rect clipa = { 0,0, 16,16 };


    int posX = 0;
    int posY = 0;
    int lig = 0;
    int col = 0;
    for (lig = 0; lig < HAUTEUR_FENTRE/TAILLE; lig++) {
        for (col = 0; col < LARGEUR_FENTRE/TAILLE; col++) {
            SDL_Rect rect;
            rect.x = posX;
            rect.y = posY;
            rect.w = TAILLE;
            rect.h = TAILLE;


            SDL_Rect clip_grass = { 0,0, 16,16 };

            SDL_Rect clip_mur = { 0,16, 16,16 };
            SDL_Rect clip_mur_casse = { 16,16, 16,16 };



            switch (game->tab[lig][col]) {
                case '.':
                    SDL_SetRenderTarget(renderer, tilemap_sol);
                    SDL_RenderCopy(renderer,tilemap_sol,&clip_grass,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'M':
                    SDL_SetRenderTarget(renderer, tilemap_sol);
                    SDL_RenderCopy(renderer,tilemap_sol,&clip_mur,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'P':
                    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'm':
                    SDL_SetRenderTarget(renderer, tilemap_sol);
                    SDL_RenderCopy(renderer,tilemap_sol,&clip_mur_casse,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'X':
                    SDL_SetRenderTarget(renderer, tilemap_sol);
                    SDL_RenderCopy(renderer,tilemap_sol,&clip_grass,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                    SDL_SetRenderTarget(renderer, tanks);
                    switch (joueur->direction) {
                        case 'N':
                            SDL_RenderCopy(renderer,tanks,&nord,&rect);
                        break;
                        case 'O':
                            SDL_RenderCopy(renderer,tanks,&ouest,&rect);
                        break;
                        case 'S':
                            SDL_RenderCopy(renderer,tanks,&sud,&rect);
                        break;
                        case 'E':
                            SDL_RenderCopy(renderer,tanks,&est,&rect);
                        break;
                    }
                    SDL_SetRenderTarget(renderer, NULL);
                    nord.x = nord.x + 16;
                    est.x = est.x + 16;
                    ouest.x = ouest.x + 16;
                    sud.x = sud.x + 16;
                    if (game->tab[lig][col+1] == '.') {
                        nord.x = 48;
                        sud.x = 0;
                        est.x = 144;
                        ouest.x = 96;
                        nord.y = nord.y + 16;
                        est.y = est.y + 16;
                        sud.y = sud.y + 16;
                        ouest.y = ouest.y + 16;
                    }
                break;
                default: {}
            }
            posX = posX + TAILLE;
        }
        posX = 0;
        posY = posY + TAILLE;
    }

}
