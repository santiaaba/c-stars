#!/bin/bash

cd libs
rm *.o
gcc -c entity.c
gcc -c button.c
gcc -c input.c
gcc -c textures.c
gcc -c playground.c
gcc -c entity.c
gcc -c game.c
cd ..
# Eliminamos el viejo compilable
rm c-start

# Compilamos y enlazamos
gcc c-start.c libs/entity.o libs/textures.o libs/game.o libs/button.o libs/input.o libs/playground.o -lpthread -lSDL2_image -lSDL2main -lSDL2 -o c-start
