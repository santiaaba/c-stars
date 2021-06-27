#include "ship.h"

void ship_init(ship_t *ship, ship_type_t type){
	ship -> power = 0;

	ship -> position = (point_t *)malloc(sizeof(point_t));
	point_init(ship -> position);

	ship -> border = (border_t *)malloc(sizeof(border_t));
	border_init(ship -> border);

	ship -> vector = (vector_t *)malloc(sizeof(vector_t));
	vector_init(ship -> vector);

	ship -> type = type;
}

void ship_set_position(ship_t *ship, point_t *point){
	point_set( ship->position,
			     point_get_x(point),
				  point_get_y(point));
}

void ship_move(ship_t *ship){
	/* Actualizamos su posicion */
	point_add_vector(ship->position,ship->vector);

	/* Actualizamos la posicion de los bordes */
	border_add_vector(ship->border,ship->vector);
}

void ship_set_vector(ship_t *ship, vector_t vector){
	vector_copy(ship->vector,vector);
}

border_t ship_border(ship_t *ship){
	return ship->border;
}

uint8_t ship_colision_ship(ship_t *ship, ship_t *ship2){
	return border_collision(ship->border,ship_border(ship2));
}

uint8_t ship_colision_shoot(ship_t *ship, shoot_t *shoot){
	return border_collision(ship->border,shoot_border(shoot));
}

void ship_border_add(ship_t *ship, int32_t x, int32_t y,
							uint32_t heigh, uint32_t width){
	rect_t *rect;
	rect = (rect_t *)malloc(sizeof(rect_t));
	rect_init(rect);
	rect_set_point(rect,x,y);
	rect_set_width(rect,width);
	rect_set_height(rect,height);
	border_add(ship->border,rect);
}

uint32_t ship_get_power(ship_t *ship){
}

void ship_set_power(ship_t *ship, uint32_t power){
	ship->power = power;
}

point_t *ship_get_position(ship_t *ship){
	return ship->position;
}

