#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "jeu.h"
#include "gestion_carte.h"

void render_tank_enemi(SDL_Renderer *renderer, const game_t *game,  const tank_t *liste) {
    tank_t * ptr = liste->nxt;
    SDL_Rect glowTankEnemi[4] = { { 0, 60, 60, 60}, { 60, 60, 60, 60}, { 120, 60, 60, 60}, { 180, 60, 60, 60}};
    SDL_Rect fullTankEnemi[4] = { { 0, 48, 48, 48}, { 48, 48, 48, 48}, { 96, 48, 48, 48}, { 144, 48, 48, 48}};
    SDL_Rect fullTankEnemi2[4] = { { 0, 96, 48, 48}, { 48, 96, 48, 48}, { 96, 96, 48, 48}, { 144, 96, 48, 48}};
    SDL_Rect fullTankEnemi3[4] = { { 0, 144, 48, 48}, { 48, 144, 48, 48}, { 96, 144, 48, 48}, { 144, 144, 48, 48}};
    SDL_Rect fullExplosionTank[4] = { { 0, 196, 48, 48}, { 48, 196, 48, 48}, { 96, 196, 48, 48}, { 144, 196, 48, 48}};
    char dirs[4] = { 'S','N','O','E'};
    while (ptr != NULL) {
        SDL_SetRenderTarget(renderer, game->textures[1]);
        SDL_Rect rectGrand = {ptr->render_pos_col + (TAILLE*18), ptr->render_pos_lig + (TAILLE*2), TAILLE*3, TAILLE*3};
        SDL_Rect rectGlow = {ptr->render_pos_col + (TAILLE*18)-GLOW_SHIFT, ptr->render_pos_lig + (TAILLE*2)-GLOW_SHIFT, GLOW_SIZE, GLOW_SIZE};
        if (ptr->etat < 4) {
            SDL_RenderCopy(renderer,game->textures[4],&glowTankEnemi[strchr(dirs, ptr->direction)-dirs],&rectGlow);
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
        SDL_Rect rect = {ptr->render_pos_col + (TAILLE*18), ptr->render_pos_lig + (TAILLE*2), TAILLE, TAILLE};

        SDL_SetRenderTarget(renderer, game->textures[0]);
        SDL_RenderCopy(renderer,game->textures[0],&obus[strchr(dirs, ptr->direction)-dirs],&rect);
        SDL_SetRenderTarget(renderer, NULL);
        ptr = ptr->nxt;
    }
}

void  render_joueur (SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur  ) {
    char dirs[4] = { 'S','N','O','E'};
    SDL_Rect glowTank[4] = { { 0, 0, 60, 60}, { 60, 0, 60, 60}, { 120, 0, 60, 60}, { 180, 0, 60, 60}};
    SDL_Rect rectGlow = {(joueur->pos_col-1)*TAILLE + (TAILLE*18)-GLOW_SHIFT, (joueur->pos_lig-1)*TAILLE + (TAILLE*2)-GLOW_SHIFT, GLOW_SIZE, GLOW_SIZE};
    SDL_Rect fullTank[4] = { { 0, 0, 48, 48}, { 48, 0, 48, 48}, { 96, 0, 48, 48}, { 144, 0, 48, 48}};
    SDL_Rect rectGrand = {(joueur->pos_col-1)*TAILLE + (TAILLE*18), (joueur->pos_lig-1)*TAILLE + (TAILLE*2), TAILLE*3, TAILLE*3};
    SDL_RenderCopy(renderer,game->textures[4],&glowTank[strchr(dirs, joueur->direction)-dirs],&rectGlow);
    SDL_SetRenderTarget(renderer, game->textures[1]);
    SDL_RenderCopy(renderer,game->textures[1],&fullTank[strchr(dirs, joueur->direction)-dirs],&rectGrand);
}

void render_vie_joueur (SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur) {
    SDL_Rect rect = { 0, 0, 32, 64 };
    SDL_Rect pos = { 2*TAILLE, 12*TAILLE, 32, 64 };
    int nbBarre = 0;
    switch (joueur->blindage) {
        case 1:
            nbBarre = 4;
        break;
        case 2:
            nbBarre = 9;
        break;
        case 3:
            nbBarre = 14;
        break;
    }


    for(int i = 0; i < nbBarre; i++)
    {
        SDL_RenderCopy(renderer, game->textures[7], &rect, &pos);
        pos.x = pos.x + 15;
    }


}

void render_tank_restant (SDL_Renderer *renderer, const game_t *game) {

    SDL_Rect rect = { 32, 32, 32, 32 };

    int x = 0;
    int y = 0;
    for (size_t i = 0; i < ( NB_TANKS - game->tanks_tue); i++) {
        SDL_Rect pos = { 2*TAILLE+(x*TAILLE*2), 38*TAILLE+(y*TAILLE*2), 2*TAILLE, 2*TAILLE};
        SDL_RenderCopy(renderer,game->textures[7],&rect,&pos);
        if (x++ > 5) { y++; x = 0; }
    }
}

void render_tab(SDL_Renderer * renderer, const game_t * game, const tank_t * joueur) {

    char ** tab = alloc_tab(LARGEUR_TAB, HAUTEUR_TAB);

    SDL_Rect rect = {TAILLE*18, TAILLE*2, TAILLE*LARGEUR_TAB, TAILLE*HAUTEUR_TAB};
    SDL_RenderCopy(renderer,game->textures[5],NULL,&rect);

    if (game->etat == EN_JEU) {
        tab = game->tab;
        render_joueur( renderer, game, joueur );
    } else {
        tab = game->tab_editeur;
    }

    SDL_Rect clip_mur[2] = { { 0, 0, 16, 16 }, { 16, 0, 16, 16 } };
    SDL_Rect clip_mur_casse[2] = { { 32, 0, 16, 16 }, { 48, 0, 16, 16 }};
    SDL_Rect clip_poussin = { 48, 16, 16, 16 };

    int posX = TAILLE*18;
    int posY = TAILLE*2;
    int lig = 0;
    int col = 0;
    int cpt = 0;

    for (lig = 0; lig < HAUTEUR_TAB; lig++) {
        for (col = 0; col < LARGEUR_TAB; col++) {
            SDL_Rect rect = {posX, posY, TAILLE, TAILLE};

            switch (tab[lig][col]) {
                case 'M':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    if (col % 2 == 0) {
                        SDL_RenderCopy(renderer,game->textures[0],&clip_mur[0],&rect);
                    } else {
                        SDL_RenderCopy(renderer,game->textures[0],&clip_mur[1],&rect);
                    }
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'B':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    SDL_RenderCopy(renderer,game->textures[0],&clip_poussin,&rect);
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                case 'm':
                    SDL_SetRenderTarget(renderer, game->textures[0]);
                    if (col % 2 == 0) {
                        SDL_RenderCopy(renderer,game->textures[0],&clip_mur_casse[0],&rect);
                    } else {
                        SDL_RenderCopy(renderer,game->textures[0],&clip_mur_casse[1],&rect);
                    }
                    SDL_SetRenderTarget(renderer, NULL);
                break;
                default:

                break;
            }

            posX = posX + TAILLE;
            cpt++;
        }
        posX = TAILLE*18;
        posY = posY + TAILLE;
    }
}

void render_game(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur, const obus_t *obus) {
    render_tab(renderer, game, joueur);
    render_tank_enemi ( renderer, game, joueur );
    render_obus ( renderer, game, obus );

}

void render_menu(SDL_Renderer *renderer, const game_t *game) {
    SDL_SetRenderTarget(renderer, game->textures[2]);
    SDL_RenderCopy(renderer,game->textures[2],NULL,NULL);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_Rect position;
    if (game->choix_menu < 3) {
         position.x = 7*TAILLE+(game->choix_menu*32*TAILLE); position.y = 20*TAILLE-TAILLE; position.w = TAILLE*22; position.h = TAILLE*10;
    } else {
        position.x = 23*TAILLE+((game->choix_menu-3)*32*TAILLE); position.y = 36*TAILLE-TAILLE; position.w = TAILLE*22; position.h = TAILLE*10;
    }
    //SDL_Rect position = { 4*TAILLE, 27*TAILLE+(game->choix_menu*5*TAILLE), TAILLE*3, TAILLE*3 };
    SDL_SetRenderTarget(renderer, game->textures[6]);
    SDL_RenderCopy(renderer,game->textures[6],NULL,&position);
    SDL_SetRenderTarget(renderer, NULL);
}

void render_editeur(SDL_Renderer *renderer, const game_t *game) {
    render_tab(renderer, game, NULL);
    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 125 );
    SDL_Rect rectGauche = { 18*TAILLE, TAILLE*2, 5*TAILLE, 5*TAILLE };
    SDL_Rect rectDroite = { LARGEUR_FENETRE-TAILLE*2-(5*TAILLE), TAILLE*2, 5*TAILLE, 5*TAILLE };
    SDL_RenderFillRect( renderer, &rectGauche );
    SDL_RenderFillRect( renderer, &rectDroite );

}

void render_gameover(SDL_Renderer *renderer, const game_t *game) {

    SDL_SetRenderDrawColor( renderer, 255, 0, 0, 125 );

}

void render_gamewon(SDL_Renderer *renderer, const game_t *game) {

    SDL_SetRenderDrawColor( renderer, 0, 255, 0, 125 );

}

void refresh_screen(SDL_Renderer * renderer, const game_t * game, const tank_t * tank_liste, const obus_t * obus_liste) {

    SDL_SetRenderDrawColor( renderer, 50, 50, 50, 255 );
    switch (game->etat) {
        case EN_JEU:
            render_game(renderer, game, tank_liste, obus_liste);
        break;
        case EN_MENU:
            render_menu(renderer, game);
        break;
        case EDITEUR:
            render_editeur(renderer, game);
        break;
        case GAME_OVER:
            render_gameover(renderer, game);
        break;
        case GAME_WON:
            render_gamewon(renderer, game);
        break;
    }


    if (game->etat != EN_MENU) {
        SDL_SetRenderTarget(renderer, game->textures[3]);
        SDL_RenderCopy(renderer,game->textures[3],NULL,NULL);
        SDL_SetRenderTarget(renderer, NULL);
        render_vie_joueur(renderer, game, tank_liste);
        render_tank_restant(renderer, game);
    }


    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderPresent(renderer);
}


void maj_obus (  obus_t * liste , const game_t * game) {
    obus_t * ptr = liste->nxt;
    int lig, col;
    while (ptr != NULL) {
        switch (ptr->direction) {
            case 'N':
                if (ptr->pos_lig >= 1) {
                    lig = ptr->pos_lig-1;
                    col = ptr->pos_col;
                    if (game->tab[lig][col] == '.') {
                        ptr->render_pos_lig -= 0.25*TAILLE;
                    }
                }
            break;
            case 'O':
                if (ptr->pos_col >= 1) {
                    lig = ptr->pos_lig;
                    col = ptr->pos_col-1;
                    if (game->tab[lig][col] == '.' ) {
                        ptr->render_pos_col -= 0.25*TAILLE;
                    }
                }
            break;
            case 'S':
                if (ptr->pos_lig < HAUTEUR_TAB-1) {
                    lig = ptr->pos_lig+1;
                    col = ptr->pos_col;
                    if (game->tab[lig][col] == '.' ) {
                        ptr->render_pos_lig += 0.25*TAILLE;
                    }
                }
            break;
            case 'E':
                if (ptr->pos_col < LARGEUR_TAB-1) {
                    lig = ptr->pos_lig;
                    col = ptr->pos_col+1;
                    if (game->tab[lig][col] == '.' ) {
                        ptr->render_pos_col += 0.25*TAILLE;
                    }
                }
            break;
        }
        ptr = ptr->nxt;
    }
}
