#ifndef LEVEL_H
#define LEVEL_H

/* Determina el nivel:
	* Bakground
	* Sonido ambiente
	* Secuencia de aparición de las naves enemigas
*/

#include "attack.h"

typedef struct t_level{
	int background;
	int soundTrak;
	lista_t *secuence;	/* Posee las secuencias de ataque */
} level_t

/* Inicializa la estructura del nivel */
void level_init(level_t *level);

/* Carga los datos del nivel desde el archivo */
void level_load(level_t *level);

/* Corre el nivel */
void level_run(level_t *level);

#endif
