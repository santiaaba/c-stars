#include "shoot.h"

void shoot_init(shoot_t *shoot){
	shoot->damage = 0;
	point_set(&(shoot->position),0,0);
	border_init(&(shoot->border));
	vector_init(&(shoot->vector));
}

void shoot_set_position(shoot_t *shoot, point_t point){
	point_copy(&(shoot->position),point);
}

void shoot_move(shoot_t *shoot){
	point_add_vector(shoot->position,shoot->vector);
}

void shoot_set_damage(shoot_t *shoot, uint32_t damage){
	shoot->damage = damage;
}

uint32_t shoot_get_damage(shoot_t *shoot){
	return shoot->damage;
}

void shoot_destroy(shoot_t *shoot){
	border_destroy(shoot->border);
}
