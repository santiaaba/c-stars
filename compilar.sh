#!/bin/bash

# Para las librerias genericas
cd libs
echo Borrando librerias genericas en /libs
rm *.o
echo Compilando librerias en /libs
gcc -c eaeapp.c
gcc -c game_info.c
gcc -c lista.c

# Para el cliente
echo Borrando librerias del cliente en /client/libs
cd ../client/libs
echo Compilando librerias del cliente en /client/libs
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
echo Compilando cliente c-start
rm c-start
# Compilamos y enlazamos
gcc c-start.c ../libs/eaeapp.o libs/entity.o libs/menu.o libs/button.o \
	libs/tcp_client.o libs/powerbar.o libs/text.o libs/textures.o \
	libs/background.o libs/game.o libs/input.o -lpthread -lSDL2_ttf \
	-lSDL2_image -lSDL2_mixer -lSDL2main -lSDL2 -o c-start --debug

# Para el servidor
echo Borrando librerias del server en /server/libs
cd ../server/libs
rm *.o
echo Compilando librerias del server en /server/libs
gcc -c point.c
gcc -c rect.c
gcc -c level.c
gcc -c border.c
gcc -c animation.c
gcc -c tcp_server.c
gcc -c clockgame.c
gcc -c eaeapp_server.c
gcc -c game.c
gcc -c ship.c
gcc -c shoot.c
gcc -c weapon.c
gcc -c vector.c

cd ..
echo Compilando server c-startd
rm c-startd
gcc c-startd.c libs/game.o libs/point.o libs/rect.o libs/border.o libs/clockgame.o \
   libs/eaeapp_server.o libs/animation.o libs/weapon.o libs/ship.o libs/level.o \
	libs/shoot.o libs/vector.o libs/tcp_server.o ../libs/eaeapp.o ../libs/lista.o \
   -lm -lpthread -lSDL2_image -lSDL2main -lSDL2 -o c-startd --debug

echo Compilando herramienta make_level
gcc make_level.c -o make_level
