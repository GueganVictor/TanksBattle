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


obus_t * creer_obus(tank_t * tank) {
    obus_t * obus = (obus_t*)malloc(sizeof(obus_t));

    obus->direction = tank->direction;
    obus->pos_lig = tank->pos_lig;
    obus->pos_col = tank->pos_col+2;

    obus->nxt = NULL;

}

void deplacer_obus (tank_t * tank, game_t * game, obus_t * liste) {
    //obus_t * enemi =  creer_obus();
    obus_t * ptr = liste->nxt;
    while (ptr!= NULL) {
        game->tab[ptr->pos_lig][ptr->pos_col] = '.';
        ptr->pos_col++;
        printf("%d - %d \n",ptr->pos_lig, ptr->pos_col );
        game->tab[ptr->pos_lig][ptr->pos_col] = 'O';
        ptr = ptr->nxt;
    }
}

void ajouter_obus (tank_t * joueur, game_t * game, obus_t * liste) {
    obus_t * obus =  creer_obus(joueur);
    obus_t * ptr = liste;
    while (ptr->nxt != NULL) {
        ptr = ptr->nxt;
        game->tab[ptr->pos_lig][ptr->pos_col] = 'O';
    }
    ptr->nxt = obus;
    printf("ajout obus\n");
    //obus_update(game, ptr->nxt, 'E');
}
