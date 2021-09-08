#include "ship.h"

void ship_init(ship_t *ship, uint8_t type, clockgame_t *clock){
	rect_t *create_rect(int32_t x, int32_t y, uint32_t width, uint32_t height){
		rect_t *newRect;
		newRect = (rect_t*)malloc(sizeof(rect_t));
		rect_init(newRect);
		rect_set_dim(newRect,width,height);
		rect_set_point(newRect,x,y);
		return newRect;
	}

	rect_t *rect;

	ship -> power = 0;
	ship -> speed = 1;

	ship -> position = (point_t *)malloc(sizeof(point_t));
	point_init(ship -> position);

	ship -> border = (border_t *)malloc(sizeof(border_t));
	border_init(ship -> border);

	ship -> vector = (vector_t *)malloc(sizeof(vector_t));
	vector_init(ship -> vector);

	ship -> ia = (ia_t *)malloc(sizeof(ia_t));
	ia_init(ship->ia, clock);

	ship -> type = type;
	ship -> ia_activated = 0;

	/* Asignamos los bordes */
	switch(type){
		case PLAYER:
			rect = create_rect(21,4,21,80);
			border_add_rect(ship->border,rect);
			rect = create_rect(42,17,17,53);
			border_add_rect(ship->border,rect);
			rect = create_rect(4,33,17,23);
			border_add_rect(ship->border,rect);
			break;
		case ENEMIE1:
			rect = create_rect(20,40,30,61);
			border_add_rect(ship->border,rect);
			rect = create_rect(50,10,56,120);
			border_add_rect(ship->border,rect);
			break;
		case ENEMIE2:
			rect = create_rect(8,19,126,23);
			border_add_rect(ship->border,rect);
			rect = create_rect(19,90,126,23);
			border_add_rect(ship->border,rect);
			rect = create_rect(30,37,114,47);
			border_add_rect(ship->border,rect);
			break;
	}
}

void ship_set_speed(ship_t *ship, float speed){
	ship->speed = speed;
}

float ship_get_speed(ship_t *ship){
	return ship->speed;
}

void ship_set_position(ship_t *ship, int32_t x, int32_t y){
	point_set( ship->position,x,y);
}

void ship_move(ship_t *ship){
	/* Modificamos el vector de movimiento
		si la ia lo ordenase */
	if(ship -> ia_activated){
		ia_drive_ship(ship->ia, ship);
	}
	/* Actualizamos su posicion */
	point_add_vector(ship->position,ship->vector);

	/* Actualizamos la posicion de los bordes */
	border_add_vector(ship->border,ship->vector);

}

void ship_set_vector(ship_t *ship, vector_t *vector){
	vector_copy(ship->vector,vector);
}

vector_t *ship_get_vector(ship_t *ship){
	return ship->vector;
}

border_t *ship_border(ship_t *ship){
	return ship->border;
}

uint8_t ship_colision_ship(ship_t *ship, ship_t *ship2){
	return border_collision(ship->border,ship_border(ship2));
}

uint8_t ship_colision_shoot(ship_t *ship, shoot_t *shoot){
	return border_collision(ship->border,shoot_get_border(shoot));
}

void ship_border_add(ship_t *ship, int32_t x, int32_t y,
							uint32_t width, uint32_t height){
	rect_t *rect;
	rect = (rect_t *)malloc(sizeof(rect_t));
	rect_init(rect);
	rect_set_point(rect,x,y);
	rect_set_dim(rect,width,height);
	border_add_rect(ship->border,rect);
}

uint32_t ship_get_power(ship_t *ship){
}

void ship_set_power(ship_t *ship, uint32_t power){
	ship->power = power;
}

point_t *ship_get_position(ship_t *ship){
	return ship->position;
}

ia_t *ship_get_ia(ship_t *ship){
	return ship -> ia;
}

void ship_ia_activate(ship_t *ship){
	ship -> ia_activated = 1;
}

void ship_destroy(ship_t **ship){
	free((*ship)->position);
	border_destroy(&((*ship)->border));
	free((*ship)->vector);
	ia_destroy(&((*ship)-> ia));
}

void ship_render(ship_t *ship, render_t *render){
	render->entity = ship->type;
	render->x = point_get_x(ship->position);
	render->y = point_get_y(ship->position);
	render->sprite = 1;		//MOMENTANEO
	render->frame = 1;		//MOMENTANEO
}

/***************************************
				PARA la IA
 ***************************************/

void ia_init(ia_t *ia, clockgame_t *clock){
	ia -> path = (lista_t *)malloc(sizeof(lista_t));
	lista_init(ia->path,sizeof(ia_mov_t));
	ia -> clock = clock;
}

void ia_start(ia_t *ia){
	lista_first(ia->path);
	ia -> time_last_mov = clockgame_time(ia->clock);
}

void ia_add_path( ia_t *ia, uint32_t instant,
					 uint32_t direction, uint32_t speed){
	vector_t *vector;
	vector = (vector_t *)malloc(sizeof(vector_t));
	vector_set(vector,direction,speed);
	lista_add(ia->path,vector);
}

void ia_drive_ship(ia_t *ia, ship_t *ship){
	ia_mov_t *aux =  lista_get(ia->path);
	if (aux != NULL)
		if((clockgame_time(ia->clock) - ia->time_last_mov) > aux->duration)
			ship_set_vector(ship, aux->vector);
}

void ia_mov_destroy(ia_mov_t **ia_mov){
	free((*ia_mov)->vector);
}

void ia_destroy(ia_t **ia){
	lista_clean((*ia)->path,&ia_mov_destroy);
	free(*ia);
	/* OJO... no eliminar el clock. Ya que es utilizado
	   por otras estructuras */
}
