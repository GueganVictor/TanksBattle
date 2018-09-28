#ifndef GAME_H_
#define GAME_H_


// VARIABLES FENETRE
#define LARGEUR_FENTRE 1600
#define HAUTEUR_FENTRE 800

// TAILLE CELLULE
#define TAILLE 16

// ETAT JEU
#define EN_COURS 0
#define FIN_JEU 1

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

typedef struct {
    char direction;
    int pos_lig;
    int pos_col;

    char provenance;
    char etat;

    struct obus_t * nxt;
} obus_t;

typedef struct {
    char ** tab;
    char ** old_tab;
    int state;
} game_t;

SDL_Texture * tilemap_sol;
SDL_Texture * tanks;

SDL_Surface *surface;
SDL_Texture *texture;
double angle = 0.0;
SDL_RendererFlip flipType;

char dirs[4] = { 'S','N','O','E'};

SDL_Rect clip_grass = { 0,0, 16,16 };
SDL_Rect clip_mur = { 0,16, 16,16 };
SDL_Rect clip_mur_casse = { 16,16, 16,16 };



#endif // GAME_H_
