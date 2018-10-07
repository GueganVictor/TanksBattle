#   include <stdio.h>
#include <stdlib.h>
#include "./model/jeu.h"



tank_t * creer_tank() {

    tank_t * tank = (tank_t*)malloc(sizeof(tank_t));
    tank->num_tank = ++nb_tank;
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

tank_t* supprimerTank(tank_t* liste, int valeur) {
    tank_t  *tmp;
    tank_t  *previous;

    if (liste == NULL) // si la listee est NULL on s'arrete tout de suite
    return (liste);
    previous = liste;
    if (previous->num_tank == valeur) { // Verifie la tete de listee, cas particulier
        liste = previous->nxt;
        free(previous);
        return (liste);
    }
    tmp = previous->nxt; // le cas n est gere on se place donc sur le cas n+1
    while(tmp != NULL) { // On Mouline est on supprime si on trouve le tank_t
        if (tmp->num_tank) {
            previous->nxt = tmp->nxt;
            free(tmp);
            return (liste);
        }
        previous = tmp; // pour ce souvenir dans la prochaine iteration du precedent
        tmp = tmp->nxt;
    }
    return liste;
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
