/* Presenta la estructura donde se almacenan
	las imágenes a utilizar en el juego */

#ifndef IMAGES_H
#define IMAGES_H

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "../libs/lista.h"

typedef struct t_images{
	SDL_Surface *background;
	lista_t *sprites;
}

void images_init(images_t *images);
void images_add_sprite(images_t *images, void sprite);
void images_get_sprite(images_t *images, uint16_t index);
void images_destroy(images_t *images);

#endif
