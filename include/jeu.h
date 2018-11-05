#ifndef JEU_H
#define JEU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// VARIABLES VITESSE
#define TICKRATE 10

// TAILLE CELLULE
#define TAILLE 16

// TAILLE TABLEAU
#define LARGEUR_TAB 100
#define HAUTEUR_TAB 50

// TAILLE FENETRE
#define LARGEUR_FENETRE LARGEUR_TAB*TAILLE
#define HAUTEUR_FENETRE HAUTEUR_TAB*TAILLE



// ETAT JEU
#define EN_JEU 0
#define EN_MENU 1
#define EDITEUR 2
#define FIN_JEU 3

// ETAT TANK
#define EN_VIE 0
#define EXPLOSION_0 1
#define EXPLOSION_1 2
#define EXPLOSION_2 3
#define EXPLOSION_3 4
#define DETRUIT 5




typedef struct tank_t tank_t;
struct tank_t {
    int num_tank;

    char direction;

    int pos_lig;
    int pos_col;

    int render_pos_lig;
    int render_pos_col;

    int blindage;

    char type;
    int etat;

    int blindage_orig;
    int nb_hit;

    struct tank_t * nxt;
};


typedef struct obus_t obus_t;
struct obus_t {

    int num_obus;

    int pos_lig;
    int pos_col;

    int render_pos_lig;
    int render_pos_col;

    char direction;
    char provenance;
    char etat;

    struct obus_t * nxt;
};

typedef struct {
    char ** tab;
    char ** tab_editeur;

    int etat;
    char case_editeur;

    int difficule;
    int choix_menu;

    int nb_tank;
    int nb_obus;

    SDL_Texture *textures[3];

    Mix_Music * music;
    Mix_Chunk * explosion;

    int cpt;
    int temps_tick;

} game_t;



#endif //
