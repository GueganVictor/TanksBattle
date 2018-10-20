#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "jeu.h"

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

void render_grass_tank ( SDL_Renderer * renderer, SDL_Rect rect, game_t * game) {
    SDL_Rect clip_grass = { 0,0, 16,16 };
    SDL_Rect r = { rect.x, rect.y,  rect.w,  rect.h  };
    for (size_t lig = 0; lig < 3; lig++) {
        for (size_t col = 0; col < 3; col++) {
            SDL_SetRenderTarget(renderer, game->textures[0]);
            SDL_RenderCopy(renderer,game->textures[0],&clip_grass,&r);
            SDL_SetRenderTarget(renderer, NULL);
            r.x += TAILLE;
        }
        r.x = rect.x;
        r.y = r.y + TAILLE;
    }

}

void render_tank_enemi(SDL_Renderer *renderer, const game_t *game,  const tank_t *liste) {
    tank_t * ptr = liste->nxt;
    SDL_Rect fullTankEnemi[4] = { { 0, 48, 48, 48}, { 48, 48, 48, 48}, { 96, 48, 48, 48}, { 144, 48, 48, 48}};
    SDL_Rect fullTankEnemi2[4] = { { 0, 96, 48, 48}, { 48, 96, 48, 48}, { 96, 96, 48, 48}, { 144, 96, 48, 48}};
    SDL_Rect fullTankEnemi3[4] = { { 0, 144, 48, 48}, { 48, 144, 48, 48}, { 96, 144, 48, 48}, { 144, 144, 48, 48}};
    char dirs[4] = { 'S','N','O','E'};
    while (ptr != NULL) {
        SDL_SetRenderTarget(renderer, game->textures[1]);
        SDL_Rect rectGrand = {(ptr->pos_col-1)*TAILLE, (ptr->pos_lig-1)*TAILLE, TAILLE*3, TAILLE*3};
        switch (ptr->blindage) {
            case 1:
                SDL_RenderCopy(renderer,game->textures[1],&fullTankEnemi[strchr(dirs, ptr->direction)-dirs],&rectGrand);
            break;
            case 2:
                SDL_RenderCopy(renderer,game->textures[1],&fullTankEnemi2[strchr(dirs, ptr->direction)-dirs],&rectGrand);
            break;
            case 3:
                SDL_RenderCopy(renderer,game->textures[1],&fullTankEnemi3[strchr(dirs, ptr->direction)-dirs],&rectGrand);
            break;
        }
        ptr = ptr->nxt;
    }
}

void render_obus(SDL_Renderer *renderer, const game_t *game,  const obus_t *liste) {
    obus_t * ptr = liste->nxt;
    SDL_Rect clip_grass = { 0,0, 16,16 };
    SDL_Rect obus[4] = { { 0, 32, 16, 16}, { 16, 32, 16, 16}, { 32, 32, 16, 16}, { 48, 32, 16, 16}};
    char dirs[4] = { 'S','N','O','E'};
    while (ptr != NULL) {
        SDL_SetRenderTarget(renderer, game->textures[0]);
        SDL_Rect rect = {(ptr->pos_col)*TAILLE, (ptr->pos_lig)*TAILLE, TAILLE, TAILLE};
        SDL_RenderCopy(renderer,game->textures[0],&clip_grass,&rect);
        SDL_RenderCopy(renderer,game->textures[0],&obus[strchr(dirs, ptr->direction)-dirs],&rect);
        SDL_SetRenderTarget(renderer, NULL);
        ptr = ptr->nxt;
    }
}

void  render_joueur (SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur  ) {
    char dirs[4] = { 'S','N','O','E'};
    SDL_Rect fullTank[4] = { { 0, 0, 48, 48}, { 48, 0, 48, 48}, { 96, 0, 48, 48}, { 144, 0, 48, 48}};
    SDL_Rect rectGrand = {(joueur->pos_col-1)*TAILLE, (joueur->pos_lig-1)*TAILLE, TAILLE*3, TAILLE*3};
    SDL_SetRenderTarget(renderer, game->textures[1]);
    SDL_RenderCopy(renderer,game->textures[1],&fullTank[strchr(dirs, joueur->direction)-dirs],&rectGrand);
}

void render_tab(SDL_Renderer * renderer, const game_t * game, const tank_t * joueur) {

    SDL_Rect clip_grass = { 0,0, 16,16 };
    SDL_Rect clip_mur = { 0,16, 16,16 };
    SDL_Rect clip_mur_casse = { 16,16, 16,16 };
    SDL_Rect clip_mur_casse2 = { 32,16, 16,16 };

    int posX = 0;
    int posY = 0;
    int oldLig = 0;
    int lig = 0;
    int col = 0;
    int cpt = 0;

    for (lig = 0; lig < HAUTEUR_FENTRE/TAILLE; lig++) {
        for (col = 0; col < LARGEUR_FENTRE/TAILLE; col++) {
            SDL_Rect rect = {posX, posY, TAILLE, TAILLE};
            SDL_Rect rectGrand = {posX, posY, TAILLE*3, TAILLE*3};

            switch (game->tab[lig][col]) {
                case '.':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    SDL_RenderCopy(renderer,game->textures[0],&clip_grass,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'M':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    SDL_RenderCopy(renderer,game->textures[0],&clip_mur,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'P':
                    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
                    SDL_RenderFillRect( renderer, &rect );
                break;
                case 'O':


                break;
                case 'm':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    SDL_RenderCopy(renderer,game->textures[0],&clip_mur_casse,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'E': case 'X':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    SDL_RenderCopy(renderer,game->textures[0],&clip_grass,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;


                default:
                break;
            }

            posX = posX + TAILLE;
            cpt++;
        }
        posX = 0;
        posY = posY + TAILLE;
    }
}

void render_game(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur, const obus_t *obus) {
    render_tab(renderer, game, joueur);
    render_joueur( renderer, game, joueur );
    render_tank_enemi ( renderer, game, joueur );
    render_obus ( renderer, game, obus );

}

void render_menu(SDL_Renderer *renderer, const game_t *game) {
    int posX = 0;
    int posY = 0;
    int lig = 0;
    int col = 0;

    // 1000 740
    SDL_Rect button = { ((LARGEUR_FENTRE/2)-250), 50, 500,370 };
    SDL_SetRenderTarget(renderer, game->textures[2]);
    SDL_RenderCopy(renderer,game->textures[2],NULL,&button);
    SDL_SetRenderTarget(renderer, NULL);
    char * texte = "Jouer au jeu.";

    SDL_Color couleur = { 0, 0, 0, 255 };
    TTF_Font * police = TTF_OpenFont("res/roboto.ttf", 121);
    SDL_Surface* surfaceText = TTF_RenderText_Blended(police, texte, couleur);
    TTF_CloseFont( police );
    SDL_Texture * stexture = SDL_CreateTextureFromSurface(renderer, surfaceText);
    SDL_Rect texted = { 10, 10, 150,90 };
    SDL_SetRenderTarget(renderer, stexture);
    SDL_RenderCopy(renderer,stexture,NULL,&texted);
    SDL_FreeSurface(surfaceText);
    SDL_DestroyTexture(stexture);

}

void render_editeur(SDL_Renderer *renderer, const game_t *game) {
    int posX = 0;
    int posY = 0;
    int lig = 0;
    int col = 0;

    render_tab(renderer, game, NULL);
}
