/* Se encarga de renderizar cada frame
	a ser mostrado en el screen */

#include <stdio.h>
#include <stdint.h>
#include "entity.h"
#include <SDL2/SDL.h>

#ifndef SCREEN_H
#define SCREEN_H

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   600
#define SCREEN_BPP      24

typedef struct t_screen{
	uint32_t last_frame;				/* Ultimo frame dibujado */
	SDL_Surface *play_ground;
	SDL_Window *window;
	entity_t buffer[100];			/* Almacena hasta 100 entidades OJO */
	uint16_t entities;				/* Tamaño logico del buffer */
} screen_t;

/* Inicializa la estructura */
void screen_init(screen_t *screen);

/* Dibuja la pantalla en base a las entidades */
static void screen_draw(screen_t *screen);

/* Genera entidades segun los datos obtenidos por el
	protocolo UDP */
void screen_add_data(screen_t *screen, char *udpdata);

/* Destruye la estructura screen */
void screen_destroy(screen_t **screen);

#endif
