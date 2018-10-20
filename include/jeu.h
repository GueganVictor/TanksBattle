#ifndef JEU_H
#define JEU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

// VARIABLES VITESSE

#define TICKRATE 100

// TAILLE CELLULE
#define TAILLE 16

#define LARGEUR_FENTRE 100*TAILLE
#define HAUTEUR_FENTRE 50*TAILLE

// ETAT JEU
#define EN_JEU 0
#define EN_MENU 1
#define EDITEUR 2
#define FIN_JEU 3

// ETAT TANK
#define EN_VIE 0
#define EXPLOSION 1
#define DETRUIT 2

// ETAT OBUS
#define EN_VIE 0
#define EXPLOSION 1


typedef struct tank_t tank_t;
struct tank_t {
    int num_tank;
    char direction;
    int pos_lig;
    int pos_col;
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
    char direction;
    int pos_lig;
    int pos_col;

    char provenance;
    char etat;

    struct obus_t * nxt;
};

typedef struct {
    char ** tab;
    int etat;
    char ** tab_editeur;
    char case_editeur;
    int nb_tank;
    int nb_obus;
    SDL_Texture *textures[3];
} game_t;



#endif //
