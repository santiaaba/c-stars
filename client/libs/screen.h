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
	last_frame uint32_t;				/* Ultimo frame dibujado */
	SQL_Surface *play_ground = NULL;
	SDL_windows *window = NULL;
	entities_t buffer[100];			/* Almacena hasta 100 entidades OJO */
	uint16_t entities;				/* Tamaño logico del buffer */
} screen_t;

/* Inicializa la estructura */
void screen_init(screen_t *screen);

/* Dibuja la pantalla en base a las entidades */
static void screen_draw(screen_t *screen);

/* Genera entidades segun los datos obtenidos por el
	protocolo UDP */
void screen_add_data(screen_t *screen, void udpdata){

/* Destruye la estructura screen */
void screen_destroy(screent_t **screen);

#endif
