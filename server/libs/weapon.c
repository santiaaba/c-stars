#include "weapon.h"

void weapon_init(weapon_t *w, uint8_t type, uint8_t frecuence,
                 clockgame_t *clock, bool automatic,
                 uint8_t probability, int x, int y, lista_t *shoots){
	w->type = type;
	w->frecuence = frecuence;
	w->clockgame = clockgame;
	w->automatic = automatic;
	w->probability = probability;
	w->last_shoot = 0;
	w->x = x;
	w->y = y;
	w->shoots = shoots;
}

bool weapon_shoot(weapon_t *w){
	shoot_t *s

	if(clockgame_time(w->clockgame) > w->last_shoot + w->frecuence){
		switch(type){
			case WEAPON1:
				s = ()
			case WEAPON2:
			case WEAPON3:
			case WEAPON4:
		}
		w->last_shoot = clockgame_time(w->clockgame);
	}
	
}
