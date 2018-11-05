#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "jeu.h"
#include "gestion_carte.h"

obus_t * creer_obus(tank_t * tank, game_t * game) {
    obus_t * obus = (obus_t*)malloc(sizeof(obus_t));
    obus->num_obus = ++game->nb_obus;
    obus->direction = tank->direction;
    switch (obus->direction) {
        case 'N':
            obus->pos_lig = tank->pos_lig-2;
            obus->pos_col = tank->pos_col;
        break;
        case 'O':
            obus->pos_lig = tank->pos_lig;
            obus->pos_col = tank->pos_col-2;
        break;
        case 'S':
            obus->pos_lig = tank->pos_lig+2;
            obus->pos_col = tank->pos_col;
        break;
        case 'E':
            obus->pos_lig = tank->pos_lig;
            obus->pos_col = tank->pos_col+2;
        break;
    }
    obus->render_pos_lig = obus->pos_lig*TAILLE;
    obus->render_pos_col = obus->pos_col*TAILLE;
    obus->nxt = NULL;

    return obus;

}

tank_t* supprimerTank(game_t * game, tank_t* liste, int valeur) {
    tank_t  *tmp;
    tank_t  *previous;

    if (liste == NULL) // si la listee est NULL on s'arrete tout de suite
    return (liste);
    previous = liste;
    tmp = previous->nxt; // le cas n est gere on se place donc sur le cas n+1
    while(tmp != NULL) { // On Mouline est on supprime si on trouve le tank_t
        if (tmp->num_tank == valeur) {
            previous->nxt = tmp->nxt;
            tank_update(game, tmp, '.');
            free(tmp);
            return (liste);
        }
        previous = tmp; // pour ce souvenir dans la prochaine iteration du precedent
        tmp = tmp->nxt;
    }
    return liste;
}

void supprimerObus(obus_t* liste, int valeur) {
    obus_t  *tmp;
    obus_t  *previous;

    if (liste == NULL) // si la listee est NULL on s'arrete tout de suite
    return;
    previous = liste;
    tmp = previous->nxt; // le cas n est gere on se place donc sur le cas n+1
    while(tmp != NULL) { // On Mouline est on supprime si on trouve le obus_t
        if (tmp->num_obus == valeur) {

            previous->nxt = tmp->nxt;
            free(tmp);
            return;
        }
        previous = tmp; // pour ce souvenir dans la prochaine iteration du precedent
        tmp = tmp->nxt;
    }
}

void change_etat_tank(tank_t * liste_tank) {
    tank_t * ptr = liste_tank->nxt;
    while (ptr != NULL) {
        if (ptr->etat != EN_VIE) {
            printf("changement etat tnak\n");
            ptr->etat++;
        }
        ptr = ptr->nxt;
    }
}

void enemi_touche(int lig, int col, game_t * game, tank_t * liste_tank) {
    printf("tank touché\n");
    tank_t * ptr = liste_tank->nxt;
    while (ptr != NULL) {
        if ( (lig >= ptr->pos_lig && lig <= ptr->pos_lig+3 ) &&  (col >= ptr->pos_col && col <= ptr->pos_col+3 ) ) {
            ptr->etat = EXPLOSION_0;
        }
        ptr = ptr->nxt;
    }
}

void supprimer_enemis(tank_t * liste_tank, game_t * game) {
    tank_t * ptr = liste_tank->nxt;
    while (ptr != NULL) {
        if (ptr->etat == DETRUIT) {
            supprimerTank(game, liste_tank, ptr->num_tank );
        }
        ptr = ptr->nxt;
    }
}

void joueur_touche(game_t * game) {
    printf("joueur touché\n");
    game->etat = FIN_JEU;
}

void deplacer_simple_obus(obus_t * obus, game_t * game, obus_t * liste, tank_t * liste_tank) {
    game->tab[obus->pos_lig][obus->pos_col] = '.';

    int lig, col;
    int del = 1;

    switch (obus->direction) {
        case 'N':
            if (obus->pos_lig < 1) {
                del = 0;
            } else {
                lig = obus->pos_lig-1;
                col = obus->pos_col;
                if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'O') {
                    obus->pos_lig = lig;
                } else if ( game->tab[lig][col] == 'm' ) {
                    game->tab[lig][col] = '.';
                    del = 0;
                } else if (game->tab[lig][col] == 'E') {
                    enemi_touche(lig-1, col, game, liste_tank);
                    del = 0;
                } else if (game->tab[lig][col] == 'X') {
                    joueur_touche(game);
                    del = 0;
                } else {
                    del = 0;
                }
            }

        break;
        case 'O':
            if (obus->pos_col < 1) {
                del = 0;
            } else {
                lig = obus->pos_lig;
                col = obus->pos_col-1;
                if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'O') {
                    obus->pos_col = col;
                } else if ( game->tab[lig][col] == 'm' ) {
                    game->tab[lig][col] = '.';
                    del = 0;
                } else if (game->tab[lig][col] == 'E') {
                    enemi_touche(lig, col-1, game, liste_tank);
                    del = 0;
                } else if (game->tab[lig][col] == 'X') {
                    joueur_touche(game);
                    del = 0;
                } else {
                    del = 0;
                }
            }
        break;
        case 'S':
            if (obus->pos_lig >= HAUTEUR_TAB-1) {
                del = 0;
            } else {
                lig = obus->pos_lig+1;
                col = obus->pos_col;
                if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'O') {
                    obus->pos_lig = lig;
                } else if ( game->tab[lig][col] == 'm' ) {
                    game->tab[lig][col] = '.';
                    del = 0;
                } else if (game->tab[lig][col] == 'E') {
                    enemi_touche(lig+1, col, game, liste_tank);
                    del = 0;
                } else if (game->tab[lig][col] == 'X') {
                    joueur_touche(game);
                    del = 0;
                } else {
                    del = 0;
                }
            }
        break;
        case 'E':
            if (obus->pos_col >= LARGEUR_TAB-1) {
                del = 0;
            } else {
                lig = obus->pos_lig;
                col = obus->pos_col+1;
                if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'O') {
                    obus->pos_col = col;
                } else if ( game->tab[lig][col] == 'm' ) {
                    game->tab[lig][col] = '.';
                    del = 0;
                } else if (game->tab[lig][col] == 'E') {
                    enemi_touche(lig, col+1, game, liste_tank);
                    del = 0;
                } else if (game->tab[lig][col] == 'X') {
                    joueur_touche(game);
                    del = 0;
                } else {
                    del = 0;
                }
            }
        break;
    }
    if (!del) {
        //printf("Suppression obus n°%d\n", obus->num_obus );
        supprimerObus(liste, obus->num_obus);
    } else {
        obus->render_pos_lig = obus->pos_lig * TAILLE;
        obus->render_pos_col = obus->pos_col * TAILLE;

        game->tab[obus->pos_lig][obus->pos_col] = 'O';
    }
}

void deplacer_obus (tank_t * liste_tank, game_t * game, obus_t * liste_obus) {
    obus_t * ptr = liste_obus->nxt;
    while (ptr!= NULL) {
        deplacer_simple_obus(ptr, game, liste_obus, liste_tank);
        ptr = ptr->nxt;
    }
}

void afficherobus(obus_t * liste) {
    obus_t * ptr = liste->nxt;
    while (ptr != NULL) {
        if (ptr->nxt == NULL) {
            printf("Obus n°%d : %d-%d | next = NULL.\n", ptr->num_obus, ptr->pos_lig, ptr->pos_col);
        } else {
            printf("Obus n°%d : %d-%d | next = %d\n", ptr->num_obus, ptr->pos_lig, ptr->pos_col, ptr->nxt->num_obus);
        }
        ptr = ptr->nxt;
    }
}

void ajouter_obus (tank_t * tank, game_t * game, obus_t * liste) {
    obus_t * obus =  creer_obus(tank, game);
    obus_t * ptr = liste;
    //printf("Ajout obus.\n");
    while (ptr->nxt != NULL) {
        ptr = ptr->nxt;
    }
    game->tab[obus->pos_lig][obus->pos_col] = 'O';
    ptr->nxt = obus;
}

void tirer_obus(tank_t * tank, game_t * game, obus_t * liste) {
    int del = 1;
    switch (tank->direction) {
        case 'N':
            if (tank->pos_lig-2 < 1) {
                del = 0;
            } else if (game->tab[tank->pos_lig-2][tank->pos_col] != '.') {
                if ( game->tab[tank->pos_lig-2][tank->pos_col] == 'm') {
                    game->tab[tank->pos_lig-2][tank->pos_col] = '.';
                    del = 0;
                } else {
                    del = 1;
                }
            }
        break;
        case 'O':
            if (tank->pos_col-2 < 1)  {
                del = 0;
            } else if (game->tab[tank->pos_lig][tank->pos_col-2] != '.') {
                if (game->tab[tank->pos_lig][tank->pos_col-2] == 'm') {
                    game->tab[tank->pos_lig][tank->pos_col-2] = '.';
                    del = 0;
                } else {
                    del = 1;
                }
            }
        break;
        case 'S':
            if (tank->pos_lig+2 > HAUTEUR_TAB-1) {
                del = 0;
            } else if (game->tab[tank->pos_lig+2][tank->pos_col] != '.') {
                if (game->tab[tank->pos_lig+2][tank->pos_col] == 'm') {
                    game->tab[tank->pos_lig+2][tank->pos_col] = '.';
                    del = 0;
                } else {
                    del = 1;
                }
            }
        break;
        case 'E':
            if (tank->pos_col+2 > LARGEUR_TAB-1) {
                del = 0;
            } else if (game->tab[tank->pos_lig][tank->pos_col+2] != '.') {
                if (game->tab[tank->pos_lig][tank->pos_col+2] == 'm') {
                    game->tab[tank->pos_lig][tank->pos_col+2] = '.';
                    del = 0;
                } else {
                    del = 1;
                }
            }
        break;
    }
    if (del) {
        ajouter_obus(tank, game,liste);
    }
}

// Tanks

tank_t * creer_tank(game_t * game) {

    tank_t * tank = (tank_t*)malloc(sizeof(tank_t));
    tank->num_tank = ++game->nb_tank;
    tank->pos_lig = 1;
    switch (rand() % 2) {
        case 0:
            tank->direction = 'E';
            tank->pos_col = 1;
        break;
        case 1:
            tank->direction = 'O';
            tank->pos_col = 95;
        break;
    }

    tank->render_pos_lig = (tank->pos_lig-1)*TAILLE;
    tank->render_pos_col = (tank->pos_col-1)*TAILLE;

    tank->blindage = 3;

    tank->type = 'E';
    tank->etat = EN_VIE;

    tank->blindage_orig = 2;
    tank->nb_hit = 0;

    tank->nxt = NULL;

    return tank;

}

void ajouter_tank (tank_t * liste, game_t * game) {
    tank_t * enemi =  creer_tank(game);
    tank_t * ptr = liste;
    while (ptr->nxt != NULL) {
        ptr = ptr->nxt;
    }
    ptr->nxt = enemi;
    tank_update(game, ptr->nxt, 'E');
}

void tirer_enemi ( tank_t * liste, game_t * game, obus_t * obus ) {
    tank_t * ptr = liste->nxt;
    while (ptr!= NULL) {
        tirer_obus(ptr, game, obus);
        ptr = ptr->nxt;
    }
}

void deplacer_tanks (tank_t * liste, game_t * game) {
    tank_t * ptr = liste->nxt;
    char dirs[4] = { 'S','N','O','E'};
    while (ptr!= NULL) {
        if (rand() % 4 != 2) {
            if (rand() % 4 == -1) {
                ptr->direction = dirs[rand() % 4];
            }
            if (ptr->etat == EN_VIE) {
                deplacer(ptr, game);
            }
        }

        ptr = ptr->nxt;
    }
}
