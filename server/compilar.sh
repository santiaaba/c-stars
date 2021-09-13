cd libs
rm point.o
gcc -c point.c
gcc -c rect.c
gcc -c level.c
gcc -c border.c
gcc -c clockgame.c
gcc -c eaeapp_server.c
gcc -c game.c
gcc -c ship.c
gcc -c shoot.c
gcc -c vector.c

cd ..
rm c-startd
gcc c-startd.c libs/game.o libs/point.o libs/rect.o libs/border.o libs/clockgame.o \
	libs/eaeapp_server.o libs/ship.o libs/level.o libs/shoot.o libs/vector.o \
	../libs/eaeapp.o ../libs/lista.o ../libs/tcp_server.o \
	-lm -lpthread -lSDL2_image -lSDL2main -lSDL2 -o c-startd --debug
