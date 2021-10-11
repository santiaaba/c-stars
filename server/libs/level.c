#include "level.h"

void static level_load(level_t *l, int id){
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
	
	printf("level_load(): Carganod el nivel\n");
	sprintf(datafile,"LEVEL%i",id);

	l->id = id;
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
		ship_init(ship,num,l->clockgame);
		ship_set_position(ship,x,y);
		fread(&num,sizeof(int16_t),1,fdIN);	/* tiempo */
		attack = (attack_t*)malloc(sizeof(attack_t));
		attack->ship = ship;
		attack->time = num;
		lista_add(l->attacks,attack);
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

void level_init(level_t *l, int id, clockgame_t *clockgame){
	printf("level_init() 2\n");
	l->attacks=(lista_t*)malloc(sizeof(lista_t));
	lista_init(l->attacks, sizeof(attack_t));
	printf("level_init() 3\n");
	l->background = 0;
	printf("level_init() 4\n");
	l->soundTrak = 0;
	printf("level_init() 5\n");
	l->clockgame = clockgame;
	printf("level_init() 6\n");
	l->state = L_INGRESS;
	printf("level_init() 7\n");
	level_load(l,id);
}

void attack_destroy(attack_t *attack){
	ship_destroy(attack->ship);
	free(attack->ship);
}

void level_destroy(level_t *l){
	lista_clean(l->attacks,(void*)(void**)&attack_destroy);
}

void level_run(level_t *l, lista_t *enemies){
	/* Realiza las acciones de arrancar un ataque o
		crear las naves segun corresponda */

	attack_t *attack;

	/* lanzamos los enemigos donde el tiempo
		de comenzar es mayor o igual al clockgame. Estan
		ordenados por tiempo de lanzamiento asi que podemos
		recorrer la lista solo hasta encontrar el que tiene
		tiempo mayor a clockgame */
	lista_first(l->attacks);
	while(!lista_eol(l->attacks) &&
			(((attack_t*)lista_get(l->attacks))->time >=
			clockgame_time(l->clockgame))){
		attack = (attack_t *)lista_remove(l->attacks);
		lista_add(enemies,attack->ship);
		free(attack);
	}
}

int level_eol(level_t *l){
	return lista_eol(l->attacks);
}

uint8_t level_get_id(level_t *l){
	return l->id;
}

void level_set_state(level_t *l, uint8_t state){
	l->state = state;
}

uint8_t level_get_state(level_t *l){
	return l->state;
}
