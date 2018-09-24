#include <stdio.h>
#include <stdlib.h>
#include "./model/jeu.h"



tank_t * creer_tank() {

    tank_t * tank = (tank_t*)malloc(sizeof(tank_t));
    if (rand() % 2 == 1) {
        tank->direction = 'E';
        tank->pos_col = 5;
    } else {
        tank->direction = 'O';
        tank->pos_col = 95;
    }

    tank->pos_lig = 5;
    tank->blindage = 2;

    tank->type = 'E';
    tank->etat = 1;

    tank->blindage_orig = 2;
    tank->nb_hit = 0;

    tank->nxt = NULL;

    return tank;

}

void ajouter_tank (tank_t * liste, game_t * game) {
    tank_t * enemi =  creer_tank();
    tank_t * ptr = liste;
    while (ptr->nxt = NULL) {
        ptr = ptr->nxt;
    }
    liste->nxt = enemi;
    tank_update(game, liste->nxt, 'E');
}

void deplacer_tanks (tank_t * liste, game_t * game) {
    tank_t * enemi =  creer_tank();
    tank_t * ptr = liste;
    while (ptr->nxt = NULL) {
        deplacer(ptr->nxt, game);
    }
}
