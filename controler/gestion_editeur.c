void get_case(int pos_lig, int pos_col, game_t * game) {
    game->tab_editeur[pos_col][pos_lig] = game->case_editeur;
}

void write_data (game_t * game) {
    FILE *fptr;
    fptr = fopen("res/editeur_map.data", "w");
    fptr = freopen("res/editeur_map.data", "w", fptr);
    if(fptr == NULL) //if file does not exist, create it
    {
        printf("Pas de fichier, création\n");
        fptr = fopen("res/editeur_map.data", "wb");
    }
    int lig, col;
    char c;
    char * output = "";
    for (lig = 0; lig < HAUTEUR_FENTRE/TAILLE; lig++) {
        for (col = 0; col < LARGEUR_FENTRE/TAILLE; col++) {
            c = game->tab_editeur[lig][col];
            fprintf(fptr, "%c", c);
        }
        fprintf(fptr, "%c",'\n' );
    }

    fclose(fptr);
}
