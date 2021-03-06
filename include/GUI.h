#ifndef GUI_H
#define GUI_H

void render_sol( SDL_Renderer * renderer, SDL_Rect rect, const game_t * game);

void render_tank_enemi(SDL_Renderer *renderer, const game_t *game,  const tank_t *liste);

void render_obus(SDL_Renderer *renderer, const game_t *game,  const obus_t *liste);

void  render_joueur (SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur  );

void render_tab(SDL_Renderer * renderer, const game_t * game, const tank_t * joueur);

void render_game(SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur, const obus_t *obus);

void render_gameover(SDL_Renderer *renderer, const game_t *game) ;

void render_menu(SDL_Renderer *renderer, const game_t *game);

void render_editeur(SDL_Renderer *renderer, const game_t *game);

void refresh_screen(SDL_Renderer * renderer, const game_t * game, const tank_t * tank_liste, const obus_t * obus_liste);

void render_tank_restant (SDL_Renderer *renderer, const game_t *game);

void render_vie_joueur (SDL_Renderer *renderer, const game_t *game,  const tank_t *joueur);

void maj_obus ( const obus_t * liste , const game_t * game);

#endif
