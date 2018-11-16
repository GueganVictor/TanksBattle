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
#define LARGEUR_TAB 80
#define HAUTEUR_TAB 46

// TAILLE FENETRE
#define LARGEUR_FENETRE 1600
#define HAUTEUR_FENETRE 800

// TANK textures

#define GLOW_SHIFT TAILLE/2
#define GLOW_SIZE 4*TAILLE

// ETAT JEU
#define EN_JEU 0
#define EN_MENU 1
#define EDITEUR 2
#define GAME_OVER 3
#define FIN_JEU 4

// ETAT TANK
#define EN_VIE 0
#define EXPLOSION_0 1
#define EXPLOSION_1 2
#define EXPLOSION_2 3
#define EXPLOSION_3 4
#define DETRUIT 5

#define FACILE 1
#define DIFFICILE 2
#define INFERNO 3
#define NON_DEFINI -1

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

typedef struct game_t game_t;
struct game_t{
    char ** tab;
    char ** tab_editeur;

    int etat;
    char case_editeur;

    int difficulte;
    int choix_menu;

    int tank_restant[3];

    int nb_tank;
    int nb_obus;

    SDL_Texture *textures[10];

    Mix_Music * music;
    Mix_Chunk * explosion;

    tank_t * tail;

    int cpt;
    int temps_tick;

};



#endif //
