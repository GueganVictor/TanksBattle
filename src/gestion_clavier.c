#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "jeu.h"
#include "gestion_carte.h"
#include "gestion_editeur.h"
#include "gestion_listes.h"
#include "dev_tools.h"


void changement_mode_editeur(game_t * game, SDL_Keycode key) {
    switch (key) {
        case SDLK_a :
            game->case_editeur = 'M';
        break;
        case SDLK_z :
            game->case_editeur = 'm';
        break;
        case SDLK_e :
            game->case_editeur = '.';
        break;
        case SDLK_r :
            game->case_editeur = 'P';
        break;
        case SDLK_ESCAPE :
            game->etat = EN_MENU;
        break;
        case SDLK_d :
            remplissage_tab_vide(HAUTEUR_TAB, LARGEUR_TAB, game->tab_editeur);
        break;
        case SDLK_s :
            printf("save\n");
            write_data(game);
            game->etat = EN_MENU;
        break;
    }
    afficher_instruction(game);
}

void changement_touche_jeu(game_t * game, tank_t * joueur, obus_t * obus, SDL_Keycode key, int * temps_tir_joueur) {
    switch (key) {
        case SDLK_UP:
            joueur->direction = 'N';
            deplacer(joueur, game);
        break;
        case SDLK_RIGHT:
            joueur->direction = 'E';
            deplacer(joueur, game);
        break;
        case SDLK_a:
            ajouter_tank(joueur, game);
        break;
        case SDLK_DOWN:
            joueur->direction = 'S';
            deplacer(joueur, game);
        break;
        case SDLK_LEFT:
            joueur->direction = 'O';
            deplacer(joueur, game);
        break;
        case SDLK_SPACE:
            if (SDL_GetTicks() > *temps_tir_joueur + 250 ) {
                Mix_PlayChannel(-1, game->explosion, 0);
                tirer_obus(joueur, game,obus);
                *temps_tir_joueur = SDL_GetTicks();
            }
        break;
        case SDLK_ESCAPE:
            game->etat = EN_MENU;
            Mix_HaltMusic();
        break;
        case SDLK_o:
            print_list_obus(obus);
        break;
        case SDLK_p:
            print_list_tank(joueur);
        break;
        case SDLK_l:
            decrease_armor(joueur);
        break;
        default:
        break;
    }

}

void changement_touche_menu(game_t * game, SDL_Keycode key) {
    switch (key) {
        case SDLK_RETURN:
            switch (game->choix_menu) {
                case 0:
                    game->etat = EN_JEU;
                    game->difficule = 1;
                    Mix_PlayMusic( game->music, -1);
                    printf("Passage en mode JEU : diff = facile\n");
                break;
                case 1:
                    game->etat = EN_JEU;
                    game->difficule = 2;
                    Mix_PlayMusic( game->music, -1);
                    printf("Passage en mode JEU : diff = difficile\n");
                break;
                case 2:
                    game->etat = EDITEUR;
                    printf("Passage en mode EDITEUR\n");
                break;
                case 3:
                    game->etat = FIN_JEU;
                    printf("Fin du jeu\n");
                break;
            }
        break;
        case SDLK_DOWN:
            if (++game->choix_menu > 3) {
                game->choix_menu = 0;
            }
        break;
        case SDLK_UP:
            if (--game->choix_menu < 0) {
                game->choix_menu = 3;
            }
        break;
    }
}
