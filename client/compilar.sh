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
gcc -lSDL2 -lSDL2_main -lSDL2_img entity.o textures.o game.o button.o input.o playground.o c-start.c -o c-start
