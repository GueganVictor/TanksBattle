# TanksBattle

## TanksBattle est un projet de l'ESIEA

### &nbsp;&nbsp;Installation :

### &nbsp;&nbsp;&nbsp;Pré-requis :

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Vous allez avoir besoin d'installer SDL2, qui permet de créer des fenêtres et en général de faire de l'affichage graphique en C et C++.

#### &nbsp;&nbsp;&nbsp;&nbsp;Si vous êtes sous linux, c'est simple lancez ces commandes :

&nbsp;&nbsp;&nbsp;&nbsp;Pour le core du SDL2 :

    apt-get install libsdl2-dev

&nbsp;&nbsp;&nbsp;&nbsp;Et pour les extensions :

    apt-get install libsdl2-image-dev libsdl2-ttf-dev libsdl2-mixer-dev

Si vous êtes sous Windows, je vous conseille de suivre ce tuto de LazyFoo Productions :

    http://lazyfoo.net/tutorials/SDL/01_hello_SDL/windows/index.php

L'application étant principalement désignée pour un système Linux, il y a des risques de bugs, incompatibilité avec Windows.

### &nbsp;&nbsp;Lancement :

### &nbsp;&nbsp;&nbsp;&nbsp;Pour lancer le jeu, faites :

    make clean; make; bin/main