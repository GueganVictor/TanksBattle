#ifndef GAME_H_
#define GAME_H_

#define TAILLE 15

#define EN_COURS 0
#define FIN_JEU 1

#define WINDOW_WIDTH 1500
#define WINDOW_HEIGHT 750

typedef struct {
    char ** tab;
    int state;
} game_t;



#endif // GAME_H_
