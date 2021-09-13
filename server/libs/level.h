#ifndef LEVEL_H
#define LEVEL_H

/* Determina el nivel:
	* Bakground
	* Sonido ambiente
	* Secuencia de aparición de las naves enemigas
*/

/* Estado de los estados */
#define L_INGRESS		0
#define L_PLAYING		1
#define L_EGRESS		2
#define L_END			3

#include <stdint.h>
#include "../../libs/lista.h"
#include "ship.h"
#include "clockgame.h"

typedef struct t_attack{
   ship_t *ship;
   uint16_t time;
} attack_t;

typedef struct t_level{
	uint8_t id;
	int background;
	int soundTrak;
	uint8_t state;
	clockgame_t *clockgame;
	lista_t *attacks;	/* Posee las secuencias de ataque */
} level_t;

/* Inicializa la estructura del nivel */
void level_init(level_t **l, int id, clockgame_t *clockgame);

/* Corre el nivel */
void level_run(level_t *l, lista_t *enemies);

/* Destruye las estructuras internas del level */
void level_destroy(level_t **l);

void level_set_state(level_t *l, uint8_t state);

uint8_t level_get_id(level_t *l);

uint8_t level_get_state(level_t *l);

int level_eol(level_t *l);

#endif
