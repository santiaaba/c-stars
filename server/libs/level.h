#ifndef LEVEL_H
#define LEVEL_H

/* Determina el nivel:
	* Bakground
	* Sonido ambiente
	* Secuencia de aparición de las naves enemigas
*/

#include "ship.h"
#include <stdint.h>
#include "../../libs/lista.h"
#include "clockgame.h"

typedef struct t_attack{
   ship_t *ship;
   uint16_t time;
} attack_t;

typedef struct t_level{
	int background;
	int soundTrak;
	clockgame_t *clockgame;
	lista_t *attacks;	/* Posee las secuencias de ataque */
} level_t;

/* Inicializa la estructura del nivel */
void level_init(level_t **level, int id, clockgame_t *clockgame);

/* Corre el nivel */
void level_run(level_t *level, lista_t *enemies);

/* Destruye las estructuras internas del level */
void level_destroy(level_t **level);

#endif
