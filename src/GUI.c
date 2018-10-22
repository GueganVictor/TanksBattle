#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "jeu.h"
#include "gestion_carte.h"

void render_sol( SDL_Renderer * renderer, SDL_Rect rect, const game_t * game) {
    SDL_Rect clip_grass[4] = { { 0, 0, 16, 16 }, { 16, 0, 16, 16 }, { 32, 0, 16, 16 }, { 48, 0, 16, 16 } };
    SDL_SetRenderTarget(renderer, game->textures[0]);
    int lig = rect.x/TAILLE;
    int col = rect.y/TAILLE;
    if ( lig % 2 == 0 && col % 2 == 0 ) {
        SDL_RenderCopy(renderer,game->textures[0],&clip_grass[0],&rect);
    } else if ( lig % 2 == 0 && col % 2 == 1 ) {
        SDL_RenderCopy(renderer,game->textures[0],&clip_grass[1],&rect);
    } else if ( lig % 2 == 1 && col % 2 == 0 ) {
        SDL_RenderCopy(renderer,game->textures[0],&clip_grass[2],&rect);
    } else {
        SDL_RenderCopy(renderer,game->textures[0],&clip_grass[3],&rect);
    }
    SDL_SetRenderTarget(renderer, NULL);
}

void render_tank_enemi(SDL_Renderer *renderer, const game_t *game,  const tank_t *liste) {
    tank_t * ptr = liste->nxt;
    SDL_Rect fullTankEnemi[4] = { { 0, 48, 48, 48}, { 48, 48, 48, 48}, { 96, 48, 48, 48}, { 144, 48, 48, 48}};
    SDL_Rect fullTankEnemi2[4] = { { 0, 96, 48, 48}, { 48, 96, 48, 48}, { 96, 96, 48, 48}, { 144, 96, 48, 48}};
    SDL_Rect fullTankEnemi3[4] = { { 0, 144, 48, 48}, { 48, 144, 48, 48}, { 96, 144, 48, 48}, { 144, 144, 48, 48}};
    SDL_Rect fullExplosionTank[4] = { { 0, 196, 48, 48}, { 48, 196, 48, 48}, { 96, 196, 48, 48}, { 144, 196, 48, 48}};
    char dirs[4] = { 'S','N','O','E'};
    while (ptr != NULL) {
        SDL_SetRenderTarget(renderer, game->textures[1]);
        SDL_Rect rectGrand = {(ptr->pos_col-1)*TAILLE, (ptr->pos_lig-1)*TAILLE, TAILLE*3, TAILLE*3};
        if (ptr->etat < 4) {
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
        }

        if (ptr->etat != EN_VIE && ptr->etat != DETRUIT) {
            SDL_RenderCopy(renderer,game->textures[1],&fullExplosionTank[ptr->etat-1],&rectGrand);
        }

        ptr = ptr->nxt;
    }
}

void render_obus(SDL_Renderer *renderer, const game_t *game,  const obus_t *liste) {
    obus_t * ptr = liste->nxt;
    SDL_Rect obus[4] = { { 0, 32, 16, 16}, { 16, 32, 16, 16}, { 32, 32, 16, 16}, { 48, 32, 16, 16}};
    char dirs[4] = { 'S','N','O','E'};
    while (ptr != NULL) {
        SDL_Rect rect = {(ptr->pos_col)*TAILLE, (ptr->pos_lig)*TAILLE, TAILLE, TAILLE};
        render_sol(renderer, rect, game);
        SDL_SetRenderTarget(renderer, game->textures[0]);
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

    char ** tab = alloc_tab(LARGEUR_TAB, HAUTEUR_TAB);
    if (game->etat == EN_JEU) {
        tab = game->tab;
    } else {
        tab = game->tab_editeur;
    }

    SDL_Rect clip_mur = { 0, 16, 16, 16 };
    SDL_Rect clip_mur_casse[2] = { { 16, 16, 16, 16 }, { 32, 16, 16, 16 }};
    SDL_Rect clip_poussin = { 48, 16, 16, 16 };

    int posX = 0;
    int posY = 0;
    int lig = 0;
    int col = 0;
    int cpt = 0;

    for (lig = 0; lig < HAUTEUR_TAB; lig++) {
        for (col = 0; col < LARGEUR_TAB; col++) {
            SDL_Rect rect = {posX, posY, TAILLE, TAILLE};

            switch (tab[lig][col]) {
                case '.':
                    render_sol(renderer, rect, game);
                break;
                case 'M':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    SDL_RenderCopy(renderer,game->textures[0],&clip_mur,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'P':
                render_sol(renderer, rect, game);
                SDL_SetRenderTarget(renderer, game->textures[0]);
                SDL_RenderCopy(renderer,game->textures[0],&clip_poussin,&rect);
                SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'm':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    if (col % 2 == 0 && lig % 2 == 0) {
                        SDL_RenderCopy(renderer,game->textures[0],&clip_mur_casse[0],&rect);
                    } else {
                        SDL_RenderCopy(renderer,game->textures[0],&clip_mur_casse[1],&rect);
                    }
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'E': case 'X':
                    render_sol(renderer, rect, game);
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
    // 1000 740
    SDL_SetRenderTarget(renderer, game->textures[2]);
    SDL_RenderCopy(renderer,game->textures[2],NULL,NULL);
    SDL_SetRenderTarget(renderer, NULL);

    SDL_Rect choix = { 48, 32, 16, 16};
    SDL_Rect position = { 4*TAILLE, 27*TAILLE+(game->choix_menu*5*TAILLE), TAILLE*3, TAILLE*3 };
    SDL_SetRenderTarget(renderer, game->textures[0]);
    SDL_RenderCopy(renderer,game->textures[0],&choix,&position);
    SDL_SetRenderTarget(renderer, NULL);

}

void render_editeur(SDL_Renderer *renderer, const game_t *game) {
    render_tab(renderer, game, NULL);

    SDL_Rect rectGauche = { 0, 0, 5*TAILLE, 5*TAILLE };
    SDL_Rect rectDroite = { LARGEUR_FENETRE-(5*TAILLE), 0, 5*TAILLE, 5*TAILLE };
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 125 );
    SDL_RenderFillRect( renderer, &rectGauche );
    SDL_RenderFillRect( renderer, &rectDroite );

}
