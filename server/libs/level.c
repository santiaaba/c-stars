#include "level.h"

void level_load(level_t *l, uint8_t id){
	FILE *fdIN;
	ia_t *ia;
	int16_t num;
	int16_t cantShips;
	int16_t cantMovements;
	int16_t i,k,x,y;
	int16_t direction, time, speed;
	attack_t *attack;
	ship_t *ship;
	char datafile[200];

	/*Borramos posibles datos de nivel viejo*/
	level_destroy(l);
	
	printf("level_load(): Cargando el nivel %i\n",id);
	sprintf(datafile,"data/level%i.data",id);

	l->id = id;
	fdIN = fopen(datafile,"rb");
	fread(&cantShips,sizeof(int16_t),1,fdIN);	/* Cantidad Naves */
	fread(&(l->background),sizeof(int16_t),1,fdIN);		/* Bakgroud */
	fread(&(l->soundTrak),sizeof(int16_t),1,fdIN);		/* Trak sound */
	printf("Cant Naves: %i - Background: %i - Sound track: %i\n",
		cantShips,l->background,l->soundTrak);
	i = 0;
	while(i < cantShips){
		fread(&num,sizeof(int16_t),1,fdIN);	/* tipo nave */
		fread(&x,sizeof(int16_t),1,fdIN);	/* coordenadaX */
		fread(&y,sizeof(int16_t),1,fdIN);	/* coordenadaY */
		ship = (ship_t*)malloc(sizeof(ship_t));
		ship_init(ship,num,l->clockgame);
		ship_set_position(ship,x,y);
		printf("Ataque nave tipo: %i, pos:(%i,%i),", num,x,y);
		fread(&num,sizeof(int16_t),1,fdIN);	/* tiempo en que se lanza*/
		printf(" tiempo: %i\n", num);
		attack = (attack_t*)malloc(sizeof(attack_t));
		attack->ship = ship;
		attack->time = num;
		lista_add(l->attacks,attack);
		fread(&cantMovements,sizeof(int16_t),1,fdIN);	/* movements */
		ia = ship_get_ia(ship);
		k = 0;
		printf("		Movimientos(%i):\n",cantMovements);
		while(k < cantMovements){
			fread(&speed,sizeof(int16_t),1,fdIN);	/* modulo */
			fread(&direction,sizeof(int16_t),1,fdIN);	/* direccion */
			fread(&time,sizeof(int16_t),1,fdIN);	/* tiempo */
			printf("			modulo:%i, direccion:%i, tiempo:%i\n",
				speed,direction,time);
			ia_add_path(ia,time,direction,speed);
			printf("Paso IA\n");
			k++;
		}
		i++;
	}
	fclose(fdIN);
	printf("Nivel cargado\n");
	l->state = L_INGRESS;
}

void level_init(level_t *l, clockgame_t *clockgame){
	l->attacks=(lista_t*)malloc(sizeof(lista_t));
	lista_init(l->attacks, sizeof(attack_t));
	l->background = 0;
	l->soundTrak = 0;
	l->clockgame = clockgame;
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
//	printf("level_run() - tamano lista: %u\n",lista_size(l->attacks));
	lista_first(l->attacks);
	if(!lista_eol(l->attacks)){
//		printf("level_run() - Primer valor time:%u , clock:%u\n",
//			((attack_t*)lista_get(l->attacks))->time,
//			clockgame_time(l->clockgame));
	}
	while(!lista_eol(l->attacks) &&
			(((attack_t*)lista_get(l->attacks))->time <=
			clockgame_time(l->clockgame))){
		attack = (attack_t *)lista_remove(l->attacks);
		ship_ia_activate(attack->ship);
		lista_add(enemies,attack->ship);
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
