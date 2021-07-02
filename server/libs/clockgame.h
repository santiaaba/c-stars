/* Es un contador de segundos que utilizaremos
	para coordinar los eventos del juego. La
	unidad es el segundo */

#ifndef CLOCKGAME_H
#define CLOCKGAME_H

#include <stdint.h>

typedef uint64_t clockgame_t;

/* Inicializa el reloj */
void clockgame_init(clockgame_t *clock);

/* retorna el tiempo actual */
uint64_t clockgame_time(clockgame_t *clock);

/* Incrementa el reloj en los segundos indicados */
void clockgame_add(clockgame_t *clock, uint64_t seconds);

#endif
