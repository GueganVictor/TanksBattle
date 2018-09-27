#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./model/jeu.h"
//#include "./GUI.h"

void render_background(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur) {
    int posX = 0;
    int posY = 0;
    int lig = 0;
    int col = 0;
    printf("affichage fond\n");

    for (lig = 0; lig < HAUTEUR_FENTRE/TAILLE; lig++) {
        for (col = 0; col < LARGEUR_FENTRE/TAILLE; col++) {
            SDL_Rect rect = {posX, posY, TAILLE, TAILLE};

            posX = posX + TAILLE;
        }
        posX = 0;
        posY = posY + TAILLE;
    }
}

void render_grass_tank ( SDL_Renderer * renderer, SDL_Rect rect) {
    SDL_Rect r = rect;
    for (size_t lig = 0; lig < 3; lig++) {
        for (size_t col = 0; col < 3; col++) {
            SDL_SetRenderTarget(renderer, tilemap_sol);
            SDL_RenderCopy(renderer,tilemap_sol,&clip_grass,&r);
            SDL_SetRenderTarget(renderer, NULL);
            r.x += TAILLE;
        }


    }

}

void render_game(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur) {

    SDL_Rect fullTank = { 0, 0, 48, 48};

    int posX = 0;
    int posY = 0;
    int oldLig = 0;
    int lig = 0;
    int col = 0;

    for (lig = 0; lig < HAUTEUR_FENTRE/TAILLE; lig++) {
        for (col = 0; col < LARGEUR_FENTRE/TAILLE; col++) {
            SDL_Rect rect = {posX, posY, TAILLE, TAILLE};
            SDL_Rect rectGrand = {posX, posY, TAILLE*3, TAILLE*3};

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
                    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'm':
                    SDL_SetRenderTarget(renderer, tilemap_sol);
                    SDL_RenderCopy(renderer,tilemap_sol,&clip_mur_casse,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'T':

                    render_grass_tank(renderer, rect);
                    //SDL_SetRenderTarget(renderer, tanks);
                    //SDL_RenderCopy(renderer,tanks,&fullTank,&rectGrand);
                    if (game->tab[lig][col] == 'E') {

                    }

                break;

                default:
                break;
            }

            posX = posX + TAILLE;
        }
        posX = 0;
        posY = posY + TAILLE;
    }

}
