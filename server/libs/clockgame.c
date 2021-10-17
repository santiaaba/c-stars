#include "clockgame.h"

void clockgame_init(clockgame_t *clockgame){
	*clockgame = 0;
}

/* retorna el tiempo actual */
uint16_t clockgame_time(clockgame_t *clockgame){
	return *clockgame;
}

/* Incrementa el reloj en los segundos indicados */
void clockgame_add(clockgame_t *clockgame, uint16_t seconds){
	*clockgame += seconds;
}
