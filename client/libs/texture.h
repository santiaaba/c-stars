/* Se encarga de almacenar las texturas necesarias para cada
	entidad y backgroud */

#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>
#include "../libs/lista.h"

typedef struct t_textures{
	SDL_Texture *background;
	lista_t *sprites;		/* Lista de SDL_Texture */
} textures_t;

void texture_init(textures_t *textures);
void texture_load_level(textures_t *textures, uint16_t level);
SDL_Texture *textures_get(textures_t *textures, uint16_t index);
void images_destroy(textures_t *textures);

#endif
