#include "weapon.h"

void weapon_init(weapon_t *weapon, uint8_t type, uint8_t frecuence,
                 clockgame_t *clockgame, bool automatic, float direction,
                 uint8_t probability, int x, int y, lista_t *shoots){
	weapon->type = type;
	weapon->clockgame = clockgame;
	weapon->automatic = automatic;
	weapon->probability = probability;
	weapon->last_shoot = 0;
	weapon->direction = direction;
	weapon->x = x;
	weapon->y = y;
	weapon->shoots = shoots;
	weapon->shooting = false;

	switch(type){
		case WEAPON_1:
			weapon->frecuence = 10;
			break;
	}
}

void weapon_shooting(weapon_t *weapon, bool shooting){
	weapon->shooting = shooting;
}

bool weapon_shoot(weapon_t *weapon){
	shoot_t *shoot;

	printf("Disparamos?: %i\n",weapon->shooting);
	if(weapon->shooting){
		if(clockgame_time(weapon->clockgame) > weapon->last_shoot + weapon->frecuence){
			shoot = (shoot_t*)malloc(sizeof(shoot_t));
			switch(weapon->type){
				case WEAPON_1:
					shoot_init(shoot,SHOOT_1,weapon->x,weapon->y,weapon->direction);
					lista_add(weapon->shoots,shoot);
			}
			weapon->last_shoot = clockgame_time(weapon->clockgame);
		}
	}
}
