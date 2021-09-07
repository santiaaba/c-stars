#include "level.h"

void static level_load(level_t *level, int id){
	FILE *fdIN;
	ia_t *ia;
	int16_t num;
	int16_t cantShips;
	int16_t cantMovements;
	int16_t i,j,k,x,y;
	int16_t direction, time, speed;
	attack_t *attack;
	ship_t *ship;
	char datafile[200];
	
	sprintf(datafile,"LEVEL%i",id);

	fdIN = fopen(datafile,"rb");
	fread(&cantShips,sizeof(int16_t),1,fdIN);	/* Cantidad Naves */
	fread(&num,sizeof(int16_t),1,fdIN);		/* Bakgroud */
	fread(&num,sizeof(int16_t),1,fdIN);		/* Trak sound */
	i = 0;
	while(j < cantShips){
		fread(&num,sizeof(int16_t),1,fdIN);	/* tipo nave */
		fread(&x,sizeof(int16_t),1,fdIN);	/* coordenadaX */
		fread(&y,sizeof(int16_t),1,fdIN);	/* coordenadaY */
		ship = (ship_t*)malloc(sizeof(ship_t));
		ship_init(ship,num,level->clockgame);
		ship_set_position(ship,x,y);
		fread(&num,sizeof(int16_t),1,fdIN);	/* tiempo */
		attack = (attack_t*)malloc(sizeof(attack_t));
		attack->ship = ship;
		attack->time = num;
		lista_add(level->attacks,attack);
		fread(&cantMovements,sizeof(int16_t),1,fdIN);	/* movements */
		ia = ship_get_ia(ship);
		k = 0;
		while(k < cantMovements){
			fread(&speed,sizeof(int16_t),1,fdIN);	/* modulo */
			fread(&direction,sizeof(int16_t),1,fdIN);	/* direccion */
			fread(&time,sizeof(int16_t),1,fdIN);	/* time */
			ia_add_path(ia,time,direction,speed);
		}
	}
	fclose(fdIN);
}

void level_init(level_t **level, int id, clockgame_t *clockgame){
	(*level) = (level_t*)malloc(sizeof(level_t));
	lista_init((*level)->attacks, sizeof(attack_t));
	(*level)->background = 0;
	(*level)->soundTrak = 0;
	(*level)->clockgame = clockgame;

	level_load((*level),id);
}



void attack_destroy(attack_t **attack){
	ship_destroy(&((*attack)->ship));
}

void level_destroy(level_t **level){
	lista_clean((*level)->attacks,&attack_destroy);
	free(*level);
}

void level_run(level_t *level, lista_t *enemies){
	/* Realiza las acciones de arrancar un ataque o
		crear las naves segun corresponda */

	attack_t *attack;

	/* lanzamos los enemigos donde el tiempo
		de comenzar es mayor o igual al clockgame. Estan
		ordenados por tiempo de lanzamiento asi que podemos
		recorrer la lista solo hasta encontrar el que tiene
		tiempo mayor a clockgame */
	lista_first(level->attacks);
	while(!lista_eol(level->attacks) &&
			(((attack_t*)lista_get(level->attacks))->time >=
			clockgame_time(level->clockgame))){
		attack = (attack_t *)lista_remove(level->attacks);
		lista_add(enemies,attack->ship);
		free(attack);
	}
}
