#ifndef GAME_H_
#define GAME_H_


// VARIABLES VITESSE

#define TICKRATE 70

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
} game_t;

SDL_Texture * tilemap_sol;
SDL_Texture * tanks;
SDL_Texture * logo;

SDL_Surface *surface;
SDL_Texture *texture;
double angle = 0.0;
SDL_RendererFlip flipType;

char dirs[4] = { 'S','N','O','E'};

SDL_Rect clip_grass = { 0,0, 16,16 };
SDL_Rect clip_mur = { 0,16, 16,16 };
SDL_Rect clip_mur_casse = { 16,16, 16,16 };



#endif // GAME_H_
