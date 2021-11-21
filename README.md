# c-stars

Juego de naves espaciales cliente servidor. UTN FRLP Internetworking

C-Star es un juego cliente servidor realizado para la cátedra de Internetworking
de la facultad Universidad Tecnológica Nacional regional La Plata.

## Lincencia

Se distribuye bajo licencia GNU

## Librerias requeridas SDL2

Para compilar los ejecutables se requieren mayormente de librerías estandard.
Pero especialmente se requiere de las librerías SDL2:

### SDL2/SDL.h

    sudo apt-get install libsdl2-dev"

### SDL2/SDL_image.h

    sudo apt-get install libsdl-image1.2-dev

### SDL2/SDL_mixer.h

    sudo apt install libsdl-mixer1.2-dev

### SDL2/SDL_ttf.h

    sudo apt-get install libsdl2-ttf-dev

## Compilacion

Ejecutar el script compilar.sh del directorio root. El mismo genera los ejecutables
dentro de los directorios server y client para el server y cliente respectivamente

Se puede optar por compilar el cliente y/o servidor por separado. Cada uno de ellos
posee su propio script compilar.hs. Pero... las librerias dentro de /libs deben
ser compiladas por separado en este caso.
