#ifndef GESTION_EDITEUR_H
#define GESTION_EDITEUR_H

void get_case(int pos_lig, int pos_col, game_t * game);

void afficher_instruction(game_t * game);

void changement_mode(game_t * game, SDL_Keycode key);

void write_data (game_t * game);

#endif
