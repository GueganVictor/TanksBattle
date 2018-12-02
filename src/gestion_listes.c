#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "jeu.h"
#include "gestion_carte.h"
#include "dev_tools.h"


// application des bonus
void appliquer_bonus (game_t * game) {
    if (game->head->blindage < 3) {
        game->head->blindage++;
    }
}
// déplacement tank
void deplacer(tank_t *tank, game_t *game ) {
    tank_update(game, tank, '.');

    int col, lig;
    switch (tank->direction) {
        case 'N':
            if (tank->pos_lig == 1) {
                break;
            } else {
                lig = tank->pos_lig-2;
                col = tank->pos_col-1;
                if (verif_deplacement(game, tank, lig, col, lig, col+1, lig, col+2)) {
                    tank->pos_lig = tank->pos_lig-1;
                }
            }

        break;
        case 'O':
            if (tank->pos_col == 1) {
                break;
            } else {
                lig = tank->pos_lig-1;
                col = tank->pos_col-2;
                if (verif_deplacement(game, tank, lig, col, lig+1, col, lig+2, col)) {
                    tank->pos_col = tank->pos_col-1;
                }
            }
        break;
        case 'S':
            if (tank->pos_lig == HAUTEUR_TAB-2) {
                break;
            } else {
                lig = tank->pos_lig+2;
                col = tank->pos_col-1;
                if (verif_deplacement(game, tank, lig, col, lig, col+1, lig, col+2)) {
                    tank->pos_lig = tank->pos_lig+1;
                }
            }
        break;
        case 'E':
            if (tank->pos_col == LARGEUR_TAB-2) {
                break;
            } else {
                lig = tank->pos_lig-1;
                col = tank->pos_col+2;
                if (verif_deplacement(game, tank, lig, col, lig+1, col, lig+2, col)) {
                    tank->pos_col = tank->pos_col+1;
                }
            }
        break;
    }
    tank_update(game, tank, 'X');
    if (tank->type == 'E') {
        tank->render_pos_lig = (tank->pos_lig - 1 ) * TAILLE;
        tank->render_pos_col = (tank->pos_col - 1 ) * TAILLE;

        tank_update(game, tank, 'E');
    }
    //show_tab_term(HAUTEUR_TAB, LARGEUR_TAB, game->tab);
}


// créer un obus
obus_t * creer_obus(tank_t * tank, game_t * game) {
    obus_t * obus = (obus_t*)malloc(sizeof(obus_t));
    obus->num_obus = ++game->nb_obus;
    obus->direction = tank->direction;
    obus->provenance = tank->type;
    obus->niveau = tank->blindage_orig;
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
// supprime le tank 
tank_t* supprimer_tank(game_t * game, tank_t* liste, int valeur) {
    tank_t  *tmp;
    tank_t  *previous;
    if (liste == NULL) // si la listee est NULL on s'arrete tout de suite
    return (liste);
    game->tanks_tue++;
    if (liste->nxt == game->tail) {
        tank_update(game, liste->nxt, '.');
        free(game->tail);
        game->tail = liste;
        return liste;
    }
    previous = liste;
    tmp = previous->nxt; // le cas n est gere on se place donc sur le cas n+1
    while(tmp != NULL) { // On Mouline est on supprime si on trouve le tank_t
        if (tmp->num_tank == valeur) {
            previous->nxt = tmp->nxt;
            tank_update(game, tmp, '.');
            free(tmp);
            return (liste);
        }
        previous = tmp; // pour se souvenir dans la prochaine iteration du precedent
        tmp = tmp->nxt;
    }

    return liste;
}
// supprime obus
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
        previous = tmp; // pour se souvenir dans la prochaine iteration du precedent
        tmp = tmp->nxt;
    }
}
// gère l'état des tanks
void change_etat_tank(tank_t * liste_tank, game_t * game) {
    if (game->head->blindage <= 0) {
        game->etat = GAME_OVER;
    }
    tank_t * ptr = liste_tank->nxt;
    while (ptr != NULL) {
        if (ptr->etat != EN_VIE) {
            ptr->etat++;
        }
        if (ptr->etat == DETRUIT) {
            supprimer_tank(game, liste_tank, ptr->num_tank);
        }
        ptr = ptr->nxt;
    }
}
// gère si l'obus touche 
void enemi_touche(int lig, int col, game_t * game, tank_t * liste_tank) {
    tank_t * ptr = liste_tank->nxt;
    int i = 0;
    while (ptr != NULL) {
        if ( (lig >= ptr->pos_lig-1 && lig <= ptr->pos_lig+2 ) &&  (col >= ptr->pos_col-1 && col <= ptr->pos_col+2 ) ) {
            i = 1;
            ptr->blindage--;
            if (ptr->blindage <= 0) {
                ptr->etat = EXPLOSION_0;
            }
        }
        ptr = ptr->nxt;
    }
    if (i == 0) {
        printf("Tank touché mais non trouvé : \n");
        printf("[DEBUG] xLig : %d ,  xCol : %d\n", lig, col);
    }
}

void joueur_touche(game_t * game) {
    game->head->blindage--;
    //game->etat = FIN_JEU;
}
// gère l'avacement de l'obus
int obus_next_case (game_t * game, obus_t * obus, tank_t * liste_tank, int lig, int col) {

    switch (game->tab[lig][col]) {
        case '.' :
        case 'O' :
        case 'A' :
            obus->pos_lig = lig;
            obus->pos_col = col;
        break;
        case 'm':
            game->tab[lig][col] = '.';
            return 0;
        break;
        case 'M':
            if (obus->niveau == 3) {
                game->tab[lig][col] = '.';
                return 0;
            }
            return 0;
        break;
        case 'E':
            if (obus->provenance == 'J') {
                switch (obus->direction){
                    case 'N':
                        enemi_touche(lig-1, col, game, liste_tank);
                    break;
                    case 'O':
                        enemi_touche(lig, col-1, game, liste_tank);
                    break;
                    case 'S':
                        enemi_touche(lig+1, col, game, liste_tank);
                    break;
                    case 'E':
                        enemi_touche(lig, col+1, game, liste_tank);
                    break;
                }
                return 0;
            }
            return 0;
        break;
        case 'X':
            joueur_touche(game);
            return 0;
        break;
        case 'B':
            game->etat = GAME_OVER;
            printf("Base touché fin du jeu");
            return 0;
        break;
        default:
            return 1;
        break;
    }
    return 1;
}
// déplacement obus
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
                del = obus_next_case(game, obus, liste_tank, lig, col);
            }

        break;
        case 'O':
            if (obus->pos_col < 1) {
                del = 0;
            } else {
                lig = obus->pos_lig;
                col = obus->pos_col-1;
                del = obus_next_case(game, obus, liste_tank, lig, col);
            }
        break;
        case 'S':
            if (obus->pos_lig >= HAUTEUR_TAB-1) {
                del = 0;
            } else {
                lig = obus->pos_lig+1;
                col = obus->pos_col;
                del = obus_next_case(game, obus, liste_tank, lig, col);
            }
        break;
        case 'E':
            if (obus->pos_col >= LARGEUR_TAB-1) {
                del = 0;
            } else {
                lig = obus->pos_lig;
                col = obus->pos_col+1;
                del = obus_next_case(game, obus, liste_tank, lig, col);
            }
        break;
    }
    if (!del) {
        supprimerObus(liste, obus->num_obus);
    } else {
        obus->render_pos_lig = obus->pos_lig * TAILLE;
        obus->render_pos_col = obus->pos_col * TAILLE;
    }
}

void deplacer_obus (tank_t * liste_tank, game_t * game, obus_t * liste_obus) {
    obus_t * ptr = liste_obus->nxt;
    while (ptr!= NULL) {
        deplacer_simple_obus(ptr, game, liste_obus, liste_tank);
        ptr = ptr->nxt;
    }
}

// ajouter des obus
void ajouter_obus (tank_t * tank, game_t * game, obus_t * liste) {
    obus_t * obus =  creer_obus(tank, game);
    obus_t * ptr = liste;
    while (ptr->nxt != NULL) {
        ptr = ptr->nxt;
    }
    ptr->nxt = obus;
}
// vérifie l'état de l'obus
int verif_tirer_obus (tank_t * tank, game_t * game, obus_t * liste, int lig, int col) {
    int del = 1;
    if ( lig < 1 || col >= LARGEUR_TAB-1 || col < 1 || lig >= HAUTEUR_TAB-1) {
        return 0;
    } else switch (game->tab[lig][col]) {
        case 'm' :
            game->tab[lig][col] = '.';
            del = 0;
        break;
        case 'E' :
            if (tank->type == 'E') {
                del = 0;
            }
        break;
        case 'M' :
            if (tank->blindage_orig == 3) {
                game->tab[lig][col] = '.';
            }
            del = 0;
        break;
        case 'B' :
            game->etat = GAME_OVER;
        break;
    }
    return del;
}
// tire 
void tirer_obus(tank_t * tank, game_t * game, obus_t * liste) {
    int del = 1;
    switch (tank->direction) {
        case 'N':
            del = verif_tirer_obus(tank, game, liste, tank->pos_lig-2, tank->pos_col);
        break;
        case 'O':
            del = verif_tirer_obus(tank, game, liste, tank->pos_lig, tank->pos_col-2);
        break;
        case 'S':
            del = verif_tirer_obus(tank, game, liste, tank->pos_lig+2, tank->pos_col);
        break;
        case 'E':
            del = verif_tirer_obus(tank, game, liste, tank->pos_lig, tank->pos_col+2);
        break;
    }
    if (del) {
        ajouter_obus(tank, game,liste);
    }
}

// Tanks

tank_t * creer_tank(game_t * game, int blindage) {

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
            tank->pos_col = LARGEUR_TAB-2;
        break;
    }

    tank->render_pos_lig = (tank->pos_lig-1)*TAILLE;
    tank->render_pos_col = (tank->pos_col-1)*TAILLE;

    tank->blindage = blindage;

    tank->type = 'E';
    tank->etat = EN_VIE;

    tank->blindage_orig = blindage;

    tank->nxt = NULL;

    return tank;

}

void ajouter_tank (tank_t * liste, game_t * game) {
    int ok = 1;
    int blindage = -1;
    int random;
    if (game->tank_restant[0] == 0 && game->tank_restant[1] == 0 && game->tank_restant[2] == 0) {
        printf("Plus de tanks à ajouter.\n");
        return;
    }
    while (ok) {
        random = rand() % 3;
        if (game->tank_restant[random] > 0) {
            blindage = random+1;
            game->tank_restant[random]--;
            ok = 0;
        }
    }
    tank_t * enemi =  creer_tank(game, blindage);
    game->tail->nxt = enemi;
    game->tail = enemi;
    tank_update(game, game->tail, 'E');
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
    char dirs[4] = { 'N','E','O','S'};
    while (ptr!= NULL) {
        if (rand() % 4 != 2) {
            if (rand() % 4 == 0) {
                int random = rand() % 8;
                if (random >= 2 ) {
                    ptr->direction = dirs[3];
                } else {
                    ptr->direction = dirs[random];
                }
            }
            if (ptr->etat == EN_VIE) {
                deplacer(ptr, game);
            }
        }

        ptr = ptr->nxt;
    }
}

// libère la mémoire de la liste des tanks sauf joueur
void free_liste_tank (tank_t * liste, game_t * game) {

    tank_t * ptr = liste->nxt;
    while (ptr != NULL) {
        supprimer_tank(game, ptr, ptr->num_tank);
        ptr = ptr->nxt;
    }

}
// libère la mémoire de la liste des obus 
void free_liste_obus (obus_t * liste, game_t * game) {

    obus_t * ptr = liste->nxt;
    while (ptr != NULL) {
        supprimerObus(ptr, ptr->num_obus);
        ptr = ptr->nxt;
    }

}