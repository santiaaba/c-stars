#!/bin/bash

cd libs
rm *.o
gcc -c entity.c
gcc -c button.c
gcc -c input.c
gcc -c powerbar.c
gcc -c background.c
gcc -c textures.c
gcc -c tcp_client.c
gcc -c entity.c
gcc -c game.c
gcc -c menu.c
gcc -c text.c
cd ..
# Eliminamos el viejo compilable
rm c-start

# Compilamos y enlazamos
gcc c-start.c ../libs/eaeapp.o libs/entity.o libs/menu.o libs/button.o libs/tcp_client.o libs/powerbar.o libs/text.o libs/textures.o libs/background.o libs/game.o libs/input.o -lpthread -lSDL2_ttf -lSDL2_image -lSDL2_mixer -lSDL2main -lSDL2 -o c-start --debug
