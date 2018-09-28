#include <stdio.h>
#include <stdlib.h>
#include "./model/jeu.h"



tank_t * creer_tank() {

    tank_t * tank = (tank_t*)malloc(sizeof(tank_t));

    tank->pos_lig = 5;
    switch (rand() % 5) {
        case 1:
            tank->direction = 'E';
            tank->pos_col = 5;
        break;
        case 2:
            tank->direction = 'O';
            tank->pos_col = 95;
        break;
        case 3:
            tank->direction = 'O';
            tank->pos_col = 95;
            tank->pos_lig = 40;
        break;
        case 4:
            tank->direction = 'E';
            tank->pos_col = 5;
            tank->pos_lig = 40;
        break;
        case 0:
            tank->direction = 'S';
            tank->pos_col = 50;
            tank->pos_lig = 25;
        break;
    }


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
    while (ptr->nxt != NULL) {
        printf("%d - %d\n", ptr->pos_col, ptr->pos_lig);
        ptr = ptr->nxt;
    }
    ptr->nxt = enemi;
    tank_update(game, ptr->nxt, 'E');
}

void deplacer_tanks (tank_t * liste, game_t * game) {
    //tank_t * enemi =  creer_tank();
    tank_t * ptr = liste->nxt;
    while (ptr!= NULL) {
        ptr->direction = dirs[rand() % 4];
        deplacer(ptr, game);
        ptr = ptr->nxt;
    }
}
