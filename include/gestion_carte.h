#ifndef GESTION_CARTE_H
#define GESTION_CARTE_H

char ** alloc_tab (int lig, int col);

void tank_update( game_t *game, tank_t *tank, char car);

void show_tab_term (int nbLig, int nbCol, char ** tab);

void deplacer(tank_t *tank, game_t *game );

int verif_deplacement ( game_t * game , tank_t * tank, int lig, int col, int lig1, int col1, int lig2, int col2 );

void remplissage_tab(int nbLig, int nbCol, char** tab, char * fichier);

char ** create_tab(int nbLig, int nbCol, char * fichier);

void remplissage_tab_vide(int nbLig, int nbCol, char** tab);

char ** create_tab_vide(int nbLig, int nbCol);

#endif
