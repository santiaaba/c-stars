#ifndef ATTACK_H
#define ATTACK_H

#include "clockgame.h"
#include "ship.h"

typedef struct t_attack{
	lista_t *enemies;
	clockgame_t *clockgame;
	uint64_t start_time;
} attack_t

/* Inicializa la estructura */
void attack_init(attack *attack_t, clockgame_t *clockgame);

/* Ejecuta el ataque */
void attack_run(attack *attack_t);

#endif
