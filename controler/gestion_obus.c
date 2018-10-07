
obus_t * creer_obus(tank_t * tank) {
    obus_t * obus = (obus_t*)malloc(sizeof(obus_t));
    obus->num_obus = ++nb_obus;
    obus->direction = tank->direction;
    obus->pos_lig = tank->pos_lig;
    obus->pos_col = tank->pos_col+2;

    obus->nxt = NULL;

    return obus;

}


void supprimerObus(obus_t* liste, int valeur) {
    obus_t  *tmp;
    obus_t  *previous;

    if (liste == NULL) // si la listee est NULL on s'arrete tout de suite
    return;
    previous = liste;
    if (previous->num_obus == valeur) { // Verifie la tete de listee, cas particulier
        liste = previous->nxt;
        free(previous);
        return;
    }
    tmp = previous->nxt; // le cas n est gere on se place donc sur le cas n+1
    while(tmp != NULL) { // On Mouline est on supprime si on trouve le obus_t
        if (tmp->num_obus) {
            previous->nxt = tmp->nxt;
            free(tmp);
            return;
        }
        previous = tmp; // pour ce souvenir dans la prochaine iteration du precedent
        tmp = tmp->nxt;
    }
}

void deplacer_obus (tank_t * tank, game_t * game, obus_t * liste) {
    obus_t * ptr = liste->nxt;
    while (ptr!= NULL) {
        if (ptr->pos_col > 95) {
            supprimerObus(liste, ptr->num_obus);
        } else {
            game->tab[ptr->pos_lig][ptr->pos_col] = '.';
            ptr->pos_col++;
            game->tab[ptr->pos_lig][ptr->pos_col] = 'O';
            ptr = ptr->nxt;
        }
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



void ajouter_obus (tank_t * tank, game_t * game, obus_t * liste) {
    obus_t * obus =  creer_obus(tank);
    obus_t * ptr = liste;
    printf("Ajout obus.\n");
    while (ptr->nxt != NULL) {
        printf("dans la boucle\n");
        ptr = ptr->nxt;
    }
    ptr->nxt = obus;
}
