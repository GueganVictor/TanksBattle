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
#include "gestion_jeu.h"
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
            game->case_editeur = 'B';
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
            printf("yo\n");
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
        case SDLK_f:
            game->etat = GAME_OVER;
            printf("Jeu arreté manuellement.");
        break;
        case SDLK_m:
            if (Mix_PlayingMusic() == 1) {
                Mix_HaltMusic();
            } else {
                Mix_PlayMusic( game->music, -1);
            }
        break;
        case SDLK_SPACE:
            if (SDL_GetTicks() > *temps_tir_joueur + 250 ) {
                Mix_PlayChannel(-1, game->effets[1], 0);
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

void valider_choix_menu(game_t * game, tank_t * liste_tank, obus_t * liste_obus) {
    switch (game->choix_menu) {
        case 0:
            game->etat = EN_JEU;
            application_difficulte(game, FACILE, liste_tank, liste_obus);
            Mix_PlayMusic( game->music, -1);
            printf("Passage en mode JEU : diff = facile\n");
        break;
        case 1:
            game->etat = EN_JEU;
            application_difficulte(game, DIFFICILE, liste_tank, liste_obus);
            game->difficulte = DIFFICILE;
            Mix_PlayMusic( game->music, -1);
            printf("Passage en mode JEU : diff = difficile\n");
        break;
        case 2:
            game->etat = EN_JEU;
            application_difficulte(game, INFERNO, liste_tank, liste_obus);
            game->difficulte = INFERNO;
            Mix_PlayMusic( game->music, -1);
            printf("Passage en mode JEU : diff = INFERNO\n");
        break;
        case 3:
            game->etat = EDITEUR;
            printf("Passage en mode EDITEUR\n");
        break;
        case 4:
            game->etat = FIN_JEU;
            printf("Fin du jeu\n");
        break;
    }
}

void changement_touche_menu(game_t * game, SDL_Keycode key, tank_t * tank, obus_t * obus ) {
    printf("Mode de jeu ? %d\n", game->choix_menu );
    if (game->choix_menu == -1) {
        game->choix_menu = 0;
        return;
    }
    switch (key) {
        case SDLK_RETURN:
            valider_choix_menu(game, tank, obus);
        break;
        case SDLK_RIGHT:
            if (++game->choix_menu > 4) {
                game->choix_menu = 0;
            }
            Mix_PlayChannel(-1, game->effets[0], 0);
        break;
        case SDLK_LEFT:
            if (--game->choix_menu < 0) {
                game->choix_menu = 4;
            }
            Mix_PlayChannel(-1, game->effets[0], 0);
        break;
        case SDLK_UP:
            if (game->choix_menu > 2) {
                game->choix_menu = 0;
            } else {
                game->choix_menu = 3;
            }
            Mix_PlayChannel(-1, game->effets[0], 0);
        break;
        case SDLK_DOWN:
            if (game->choix_menu < 3) {
                game->choix_menu = 3;
            } else {
                game->choix_menu = 0;
            }
            Mix_PlayChannel(-1, game->effets[0], 0);
        break;
    }
}

void deplacement_souris_menu(game_t * game, int x, int y) {
    if (x > 8*TAILLE && x < 28*TAILLE && y > 20*TAILLE && y < 28*TAILLE) {
        game->choix_menu = 0;
    } else if (x > 40*TAILLE && x < 60*TAILLE && y > 20*TAILLE && y < 28*TAILLE) {
        game->choix_menu = 1;
    } else if (x > 72*TAILLE && x < 92*TAILLE && y > 20*TAILLE && y < 28*TAILLE) {
        game->choix_menu = 2;
    } else if (x > 24*TAILLE && x < 44*TAILLE && y > 36*TAILLE && y < 60*TAILLE) {
        game->choix_menu = 3;
    } else if (x > 56*TAILLE && x < 76*TAILLE && y > 36*TAILLE && y < 60*TAILLE) {
        game->choix_menu = 4;
    } else {
        game->choix_menu = -1;
    }

}

void changement_touche_fin_jeu(game_t * game, SDL_Keycode key, tank_t * tank_liste, obus_t * liste_obus){
    Mix_HaltMusic();
     switch (key) {
        case SDLK_RETURN:
        case SDLK_ESCAPE:
            game->etat = EN_MENU;
            reset_game(game, tank_liste, liste_obus);
        break;
     }

}


    /*


void deplacement_souris_menu(game_t * game, int x, int y) {
    if (x > 8*TAILLE && x < 8*TAILLE+20*TAILLE && y > 20*TAILLE && y < 20*TAILLE+8*TAILLE) {
        game->choix_menu = 0;
    }
    if (x > 8*TAILLE+ (32*TAILLE) && x < 32*TAILLE+8*TAILLE+20*TAILLE && y > 20*TAILLE && y < 20*TAILLE+8*TAILLE) {
        game->choix_menu = 1;
    }
    if (x > 8*TAILLE+ (64*TAILLE) && x < 64*TAILLE+8*TAILLE+20*TAILLE && y > 20*TAILLE && y < 20*TAILLE+8*TAILLE) {
        game->choix_menu = 2;
    }
    if (x > 24*TAILLE && x < 24*TAILLE+20*TAILLE && y > 36*TAILLE && y < 36*TAILLE+24*TAILLE) {
        game->choix_menu = 3;
    }
    if (x > 24*TAILLE+ (32*TAILLE) && x < 32*TAILLE+24*TAILLE+20*TAILLE && y > 36*TAILLE && y < 36*TAILLE+24*TAILLE) {
        game->choix_menu = 4;
    }
}
*/
