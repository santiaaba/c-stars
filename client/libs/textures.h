/* Se encarga de almacenar las texturas necesarias para cada
	entidad y backgroud */

#ifndef TEXTURES_H
#define TEXTURES_H

#include <stdio.h>
#include <stdint.h>
#include <SDL2/SDL.h>

typedef struct t_textures{
	SDL_Texture *background;
	SDL_Texture *sprites[10];	/* Permite hasta 10 texturas */
	uint16_t size;					/* Tamano logico del vector sprites*/
} textures_t;

void texture_init(textures_t *textures);
void texture_load_level(textures_t *textures, uint16_t level);
SDL_Texture *textures_get(textures_t *textures, uint16_t index);
void texture_destroy(textures_t *textures);

#endif
