#include "weapon.h"

void weapon_init(weapon_t *weapon, uint8_t type, uint8_t frecuence,
                 clockgame_t *clockgame, bool automatic, float direction,
                 uint8_t probability, point_t *position, int offset_x,
					  int offset_y, lista_t *shoots){
	weapon->type = type;
	weapon->clockgame = clockgame;
	weapon->automatic = automatic;
	weapon->probability = probability;
	weapon->last_shoot = 0;
	weapon->direction = direction;
	weapon->position = position;
	weapon->offset_x = offset_x;
	weapon->offset_y = offset_y;
	weapon->shoots = shoots;
	weapon->shooting = false;

	switch(type){
		case WEAPON_1:
			weapon->frecuence = 2;
			break;
	}
}

void weapon_shooting(weapon_t *weapon, bool shooting){
	weapon->shooting = shooting;
}

point_t *weapon_get_position(weapon_t *weapon){
	return weapon->position;
}

bool weapon_shoot(weapon_t *weapon){
	shoot_t *shoot;

	//printf("Disparamos?: %i\n",weapon->shooting);
	if(weapon->shooting){
		//printf("weapon_shoot(): clock:%i > next:%i\n",clockgame_time(weapon->clockgame), weapon->last_shoot + weapon->frecuence);
		if(clockgame_time(weapon->clockgame) > weapon->last_shoot + weapon->frecuence){
			//printf("Creamos un nuevo disparo\n");
			shoot = (shoot_t*)malloc(sizeof(shoot_t));
			switch(weapon->type){
				case WEAPON_1:
					//printf("Iniciamos el disparo\n");
					shoot_init(shoot,SHOOT_1,
						point_get_x(weapon->position) + weapon -> offset_x,
						point_get_y(weapon->position) + weapon -> offset_y,
						weapon->direction);
					//printf("Agregamos el disparo a la lista\n");
					lista_add(weapon->shoots,shoot);
				default:
					printf("Error fatal. No es un WEAPON\n");
			}
			//printf("Registramos el tiempo del ultimo disparo\n");
			weapon->last_shoot = clockgame_time(weapon->clockgame);
		}
	}
}
