/* Se encarga de renderizar la pantalla del juego */

#include <stdio.h>
#include <stdint.h>
#include "entity.h"
#include <SDL2/SDL.h>

#ifndef PLAYGROUND_H
#define PLAYGROUND_H

#define ENTITIES_LIMIT 100

typedef struct t_playground{
	uint32_t last_frame;						/* Ultimo frame dibujado */
	entity_t buffer[ENTITIES_LIMIT];		/* Almacena hasta 100 entidades OJO */
	uint16_t entities;						/* Tamaño logico del buffer */
} playground_t;

/* Inicializa la estructura */
int playground_init(playground_t *playground);

/* Dibuja la pantalla en base a las entidades */
static void playground_draw(playground_t *playground, int clear, SDL_Renderer *renderer);

/* Borra logicamente el array de entidades */
void playground_clear_entities(playground_t *playground);

/* Genera entidades segun los datos obtenidos por el
	protocolo UDP */
void playground_add_entity(playground_t *playground, entity_t *entity);

#endif
