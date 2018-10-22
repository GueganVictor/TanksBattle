#ifndef GESTION_CLAVIER_H
#define GESTION_CLAVIER_H

void changement_mode_editeur(game_t * game, SDL_Keycode key);

void changement_touche_menu(game_t * game, SDL_Keycode key);

void changement_touche_jeu(game_t * game, tank_t * joueur, obus_t * obus, SDL_Keycode key, int temps_tir_joueur);

#endif
