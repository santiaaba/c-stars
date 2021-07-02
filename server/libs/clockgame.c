#include "clockgame.h"

void clockgame_init(clockgame_t *clock){
	*clock = 0;
}

/* retorna el tiempo actual */
uint64_t clockgame_time(clockgame_t *clock){
	return *clock;
}

/* Incrementa el reloj en los segundos indicados */
void clockgame_add(clockgame_t *clock, uint64_t seconds){
	*clock += seconds;
}
