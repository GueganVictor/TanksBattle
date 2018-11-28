#ifndef GESTION_OBUS_H
#define GESTION_OBUS_H

void free_liste_tank (tank_t * liste, game_t * game);

void free_liste_obus (obus_t * liste, game_t * game);

obus_t * creer_obus(tank_t * tank);

void enemi_touche(int lig, int col, game_t * game, tank_t * liste_tank);

void joueur_touche(game_t * game);

void supprimerObus(obus_t* liste, int valeur);

void deplacer_simple_obus(obus_t * obus, game_t * game, obus_t * liste, tank_t * liste_tank);

void change_etat_tank(tank_t * liste_tank, game_t * game);

void deplacer_obus (tank_t * tank, game_t * game, obus_t * liste);

void afficherobus(obus_t * liste);

void ajouter_obus (tank_t * tank, game_t * game, obus_t * liste);

void tirer_obus(tank_t * tank, game_t * game, obus_t * liste);

tank_t * creer_tank(game_t * game, int blindage);

void ajouter_tank (tank_t * liste, game_t * game);

void tirer_enemi ( tank_t * liste, game_t * game, obus_t * obus );

void deplacer_tanks (tank_t * liste, game_t * game);

void maj_tank (  tank_t * liste , const game_t * game);

#endif
