#include "weapon.h"

void weapon_init(weapon_t *weapon, uint8_t type, uint8_t frecuence,
                 clockgame_t *clockgame, float direction, bool automatic,
                 uint8_t probability, point_t *position, int offset_x,
					  int offset_y, lista_t *shoots){
	weapon->type = type;
	weapon->clockgame = clockgame;
	weapon->probability = probability;
	weapon->automatic = automatic;
	weapon->last_shoot = 0;
	weapon->direction = direction;
	weapon->position = position;
	weapon->offset_x = offset_x;
	weapon->offset_y = offset_y;
	weapon->shoots = shoots;
	weapon->shooting = false;
	weapon->frecuence = frecuence;
}

void weapon_shooting(weapon_t *weapon, bool shooting){
	weapon->shooting = shooting;
}

bool weapon_is_shooting(weapon_t *weapon){
	return weapon->shooting;
}

point_t *weapon_get_position(weapon_t *weapon){
	return weapon->position;
}

bool weapon_shoot(weapon_t *weapon){
	shoot_t *shoot;
	bool aux;

	if(weapon->automatic)
		aux = (rand()%100 < weapon->probability);
	else
		aux =true;

	//printf("Disparamos?: %i\n",weapon->shooting);
	if(weapon->shooting){
//		printf("weapon_shoot(): clock:%i > next: %i + %i = %i\n",clockgame_time(weapon->clockgame),
//			weapon->last_shoot, weapon->frecuence, weapon->last_shoot + weapon->frecuence);
		if(clockgame_time(weapon->clockgame) > weapon->last_shoot + weapon->frecuence && aux){
			//printf("Creamos un nuevo disparo\n");
			shoot = (shoot_t*)malloc(sizeof(shoot_t));
//			printf("weapon_shoot(): %i\n",weapon->type);
			switch(weapon->type){
				case WEAPON_1:
					//printf("Iniciamos el disparo\n");
					shoot_init(shoot,SHOOT_1,
						point_get_x(weapon->position) + weapon -> offset_x,
						point_get_y(weapon->position) + weapon -> offset_y,
						weapon->direction);
					//printf("Agregamos el disparo a la lista\n");
					lista_add(weapon->shoots,shoot);
					break;
				default:
					printf("weapon_shoot():Error fatal. No es un WEAPON\n");
			}
			//printf("Registramos el tiempo del ultimo disparo\n");
			weapon->last_shoot = clockgame_time(weapon->clockgame);
		}
	}
}
