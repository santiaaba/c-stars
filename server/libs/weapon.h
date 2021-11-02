#ifndef WAAPON_H
#define WEAPON_H

#define WEAPON_1		1		/* punto de energia */

#include "../../libs/lista.h"
#include "../../libs/types.h"
#include "./clockgame.h"
#include "./shoot.h"
#include "./point.h"

typedef struct {
	uint8_t type;
	uint8_t frecuence;
	clockgame_t *clockgame;
	point_t *position;		/* Es el puntero posición de la nave */
	int offset_x;				/* Es el desplazamiento relativo en x */
	int offset_y;				/* Es el desplazamiento relativo en y */
	bool automatic;
	uint8_t probability;		/* Probabilidad de disparo cuando es automatico */
	uint16_t last_shoot;		/* Ultimo frame en que se disparo */
	float direction;			//direccion en radianes
	bool shooting;
	lista_t *shoots;
} weapon_t;

void weapon_init(weapon_t *weapon, uint8_t type, uint8_t frecuence,
					  clockgame_t *clock, bool automatic, float direction,
					  uint8_t probability, point_t *position, int offset_x,
					  int offset_y, lista_t *shoots);
void weapon_shooting(weapon_t *weapon, bool shooting);
point_t *weapon_get_position(weapon_t *weapon);
bool weapon_shoot(weapon_t *weapon);

#endif
