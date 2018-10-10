
obus_t * creer_obus(tank_t * tank) {
    obus_t * obus = (obus_t*)malloc(sizeof(obus_t));
    obus->num_obus = ++nb_obus;
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

    obus->nxt = NULL;

    return obus;

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

void deplacer_simple_obus(obus_t * obus, game_t * game, obus_t * liste) {
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
                } else {
                    del = 0;
                }
            }
        break;
        case 'S':
            if (obus->pos_lig >= HAUTEUR_FENTRE/TAILLE-1) {
                del = 0;
            } else {
                lig = obus->pos_lig+1;
                col = obus->pos_col;
                if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'O') {
                    obus->pos_lig = lig;
                } else if ( game->tab[lig][col] == 'm' ) {
                    game->tab[lig][col] = '.';
                    del = 0;
                } else {
                    del = 0;
                }
            }
        break;
        case 'E':
            if (obus->pos_col >= LARGEUR_FENTRE/TAILLE-1) {
                del = 0;
            } else {
                lig = obus->pos_lig;
                col = obus->pos_col+1;
                if (game->tab[lig][col] == '.' || game->tab[lig][col] == 'O') {
                    obus->pos_col = col;
                } else if ( game->tab[lig][col] == 'm' ) {
                    game->tab[lig][col] = '.';
                    del = 0;
                } else {
                    del = 0;
                }
            }
        break;
    }
    if (!del) {
        printf("Suppression obus n°%d\n", obus->num_obus );
        supprimerObus(liste, obus->num_obus);
    } else {
        game->tab[obus->pos_lig][obus->pos_col] = 'O';
    }
}

void deplacer_obus (tank_t * tank, game_t * game, obus_t * liste) {
    obus_t * ptr = liste->nxt;
    while (ptr!= NULL) {
        deplacer_simple_obus(ptr, game, liste);
        ptr = ptr->nxt;
    }
}

void afficherobus(obus_t * liste) {
    obus_t * ptr = liste->nxt;
    while (ptr != NULL) {
        if (ptr->nxt == NULL) {
            printf("Obus n°%d : %d-%d | next = NULL.\n", ptr->num_obus, ptr->pos_lig, ptr->pos_col);
            /* code */
        } else {
            printf("Obus n°%d : %d-%d | next = %d\n", ptr->num_obus, ptr->pos_lig, ptr->pos_col, ptr->nxt->num_obus);
        }
        ptr = ptr->nxt;
    }
}

void printlist(obus_t * liste) {
    obus_t * ptr = liste->nxt;
    while (ptr != NULL) {
        printf("Obus n°%d | ", ptr->num_obus);
        ptr = ptr->nxt;
    }
    printf("\n");
}


void ajouter_obus (tank_t * tank, game_t * game, obus_t * liste) {
    obus_t * obus =  creer_obus(tank);
    obus_t * ptr = liste;
    printf("Ajout obus.\n");
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
            }
            if (game->tab[tank->pos_lig-2][tank->pos_col] != '.') {
                if ( game->tab[tank->pos_lig-2][tank->pos_col] == 'm') {
                    game->tab[tank->pos_lig-2][tank->pos_col] = '.';
                    del = 0;
                } else {
                    del = 1;
                }/* code */
            }
        break;
        case 'O':
            if (tank->pos_col-2 < 1)  {
                del = 0;
            }
            if (game->tab[tank->pos_lig][tank->pos_col-2] != '.') {
                if (game->tab[tank->pos_lig][tank->pos_col-2] == 'm') {
                    game->tab[tank->pos_lig][tank->pos_col-2] = '.';
                    del = 0;
                } else {
                    del = 1;
                }/* code */
            }
        break;
        case 'S':
            if (tank->pos_lig+2 > HAUTEUR_FENTRE/TAILLE-1) {
                del = 0;
            }
            if (game->tab[tank->pos_lig+2][tank->pos_col] != '.') {
                if (game->tab[tank->pos_lig+2][tank->pos_col] == 'm') {
                    game->tab[tank->pos_lig+2][tank->pos_col] = '.';
                    del = 0;
                } else {
                    del = 1;
                }/* code */
            }
        break;
        case 'E':
            if (tank->pos_col+2 > LARGEUR_FENTRE/TAILLE-1) {
                del = 0;
            }
            if (game->tab[tank->pos_lig][tank->pos_col+2] != '.') {
                if (game->tab[tank->pos_lig][tank->pos_col+2] == 'm') {
                    game->tab[tank->pos_lig][tank->pos_col+2] = '.';
                    del = 0;
                } else {
                    del = 1;
                }/* code */
            }
        break;
    }
    if (del) {
        ajouter_obus(tank, game,liste);
    }
}
