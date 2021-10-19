#ifndef WAAPON_H
#define WEAPON_H

typedef struct {
	uint8_t type;
	uint8_t frecuence;
	clockgame_t *clock;
	bool automatic;
	uint8_t probability;
	uint16_t last_shoot;
	int x;
	int y;
	lista_t *shoots;
} weapon_t

void weapon_init(weapon_t *w, uint8_t type, uint8_t frecuence,
					  clockgame_t *clock, bool automatic,
					  uint8_t probability, int x, int y, lista_t *shoots);
bool weapon_shoot();

#endif
