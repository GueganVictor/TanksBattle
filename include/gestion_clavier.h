#ifndef GESTION_CLAVIER_H
#define GESTION_CLAVIER_H

void changement_mode_editeur(game_t * game, SDL_Keycode key);

void changement_touche_menu(game_t * game, SDL_Keycode key, tank_t * liste_tank, obus_t * liste_obus );

void changement_touche_jeu(game_t * game, tank_t * joueur, obus_t * obus, SDL_Keycode key, int * temps_tir_joueur);

void deplacement_souris_menu(game_t * game, int x, int y);

void valider_choix_menu(game_t * game, tank_t * liste_tank, obus_t * liste_obus);

void changement_touche_fin_jeu(game_t * game, SDL_Keycode key, tank_t * tank_liste, obus_t * liste_obus);

#endif
