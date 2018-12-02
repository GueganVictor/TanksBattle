void reset_game (game_t * game, tank_t * liste_tank, obus_t * liste_obus);

void application_difficulte(game_t * game, int difficulte, tank_t * liste_tank, obus_t * liste_obus);

void verif_victoire (game_t * game);

void init_image (SDL_Renderer * renderer, game_t * game);

int init_sons (game_t * game);

void ajouter_bonus (game_t * game);