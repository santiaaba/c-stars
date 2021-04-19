cd libs
rm point.o
gcc -c point.c

cd ..
gcc point.o main.c
