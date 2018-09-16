cible:


hello: main.c model/jeu.h
	gcc main.c -I. -o exo  `sdl2-config --cflags --libs ` -lSDL2_image
