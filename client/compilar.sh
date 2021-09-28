#!/bin/bash

cd libs
rm *.o
gcc -c entity.c
gcc -c button.c
gcc -c input.c
gcc -c textures.c
gcc -c tcp_client.c
gcc -c playground.c
gcc -c entity.c
gcc -c game.c
gcc -c text.c
cd ..
# Eliminamos el viejo compilable
rm c-start

# Compilamos y enlazamos
gcc c-start.c ../libs/eaeapp.o libs/entity.o libs/button.o libs/tcp_client.o libs/text.o libs/textures.o libs/game.o libs/input.o libs/playground.o -lpthread -lSDL2_ttf -lSDL2_image -lSDL2main -lSDL2 -o c-start --debug
