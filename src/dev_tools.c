#include "jeu.h"

void print_list_obus(obus_t * liste) {
    obus_t * ptr = liste->nxt;
    while (ptr != NULL) {
        printf("Obus n°%d | ", ptr->num_obus);
        ptr = ptr->nxt;
    }
    printf("\n");
}

void print_list_tank(tank_t * liste) {
    tank_t * ptr = liste->nxt;
    while (ptr != NULL) {
        printf("Tank n°%d en %d:%d ", ptr->num_tank, ptr->pos_lig, ptr->pos_col);
        ptr = ptr->nxt;
    }
    printf("\n");
}

void decrease_armor(tank_t * liste) {
    tank_t * ptr = liste->nxt;
    while (ptr != NULL) {
        ptr->blindage--;
        ptr = ptr->nxt;
    }
}

void increase_armor(tank_t * liste) {
    tank_t * ptr = liste->nxt;
    while (ptr != NULL) {
        ptr->blindage++;
        ptr = ptr->nxt;
    }
}
