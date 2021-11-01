#ifndef WAAPON_H
#define WEAPON_H

#define WEAPON_1		1		/* punto de energia */

#include "../../libs/lista.h"
#include "../../libs/types.h"
#include "./clockgame.h"
#include "./shoot.h"

typedef struct {
	uint8_t type;
	uint8_t frecuence;
	clockgame_t *clockgame;
	bool automatic;
	uint8_t probability;
	uint16_t last_shoot;
	int x;
	int y;
	float direction;		//direccion en radianes
	bool shooting;
	lista_t *shoots;
} weapon_t;

void weapon_init(weapon_t *weapon, uint8_t type, uint8_t frecuence,
					  clockgame_t *clock, bool automatic, float direction,
					  uint8_t probability, int x, int y, lista_t *shoots);
void weapon_shooting(weapon_t *weapon, bool shooting);
bool weapon_shoot(weapon_t *weapon);

#endif
