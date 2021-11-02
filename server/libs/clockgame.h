/* Es un contador que utilizaremos
	para coordinar los eventos del juego. La
	unidad es la decima de segundo */

#ifndef CLOCKGAME_H
#define CLOCKGAME_H

#define CLOCK_RUN		0
#define CLOCK_STOP	1

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct{
	pthread_t th;
	int state;
	uint32_t time;		// Decimas de segundo
} clockgame_t;

/* Inicializa el reloj */
void clockgame_init(clockgame_t *clockgame);
/* retorna el tiempo actual */
uint32_t clockgame_time(clockgame_t *clockgame);
/* Incrementa el reloj en los segundos indicados */
void clockgame_restore(clockgame_t *clockgame);
void clockgame_start(clockgame_t *clockgame);
void clockgame_stop(clockgame_t *clockgame);

/*
void clockgame_add(clockgame_t *clockgame, uint32_t seconds);
*/

#endif
