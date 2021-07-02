#include "attack.h"

void attack_init(attack *attack_t, clockgame_t *clockgame){
	attack->enemies = (lista_t *)malloc(sizeof(lista_t));
	lista_init(attack->enemies);
	attack->clockgame = clockgame;
}

void attack_run(attack *attack_t){
}
