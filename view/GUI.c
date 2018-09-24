#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "./model/jeu.h"
//#include "./GUI.h"


void render_game(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur) {

    SDL_Rect textu;
    textu.x = 0;
    textu.y = 0;
    textu.w = 10;
    textu.h = 10;

    int cptTexture = 0;

    SDL_Rect nord = { 48, 0, 16, 16};
    SDL_Rect ouest = { 96, 0, 16, 16};
    SDL_Rect est = { 144, 0, 16, 16};
    SDL_Rect sud = { 0, 0, 16, 16};

    SDL_Rect nordE = { 48, 48, 16, 16};
    SDL_Rect ouestE = { 96, 48, 16, 16};
    SDL_Rect estE = { 144, 48, 16, 16};
    SDL_Rect sudE = { 0, 48, 16, 16};

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
                    SDL_SetRenderDrawColor( renderer, 255, 255, 0, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'E':
                    SDL_SetRenderTarget(renderer, tilemap_sol);
                    SDL_RenderCopy(renderer,tilemap_sol,&clip_grass,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                    SDL_SetRenderTarget(renderer, tanks);
                    switch (joueur->nxt->direction) {
                        case 'N':
                            SDL_RenderCopy(renderer,tanks,&nordE,&rect);
                        break;
                        case 'O':
                            SDL_RenderCopy(renderer,tanks,&ouestE,&rect);
                        break;
                        case 'S':
                            SDL_RenderCopy(renderer,tanks,&sudE,&rect);
                        break;
                        case 'E':
                            SDL_RenderCopy(renderer,tanks,&estE,&rect);
                        break;
                    }
                    SDL_SetRenderTarget(renderer, NULL);
                    nordE.x = nordE.x + 16;
                    estE.x = estE.x + 16;
                    ouestE.x = ouestE.x + 16;
                    sudE.x = sudE.x + 16;
                    cptTexture++;
                    if (cptTexture > 2) {
                        nordE.x = 48;
                        sudE.x = 0;
                        estE.x = 144;
                        ouestE.x = 96;
                        nordE.y = nordE.y + 16;
                        estE.y = estE.y + 16;
                        sudE.y = sudE.y + 16;
                        ouestE.y = ouestE.y + 16;
                        cptTexture = 0;
                    }
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
                    cptTexture++;
                    if (cptTexture > 2) {
                        nord.x = 48;
                        sud.x = 0;
                        est.x = 144;
                        ouest.x = 96;
                        nord.y = nord.y + 16;
                        est.y = est.y + 16;
                        sud.y = sud.y + 16;
                        ouest.y = ouest.y + 16;
                        cptTexture = 0;
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
