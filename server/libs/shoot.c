#include "shoot.h"

void shoot_init(shoot_t *shoot, int type){
	shoot->type = type;
	shoot->damage = 0;
	point_set(shoot->position,0,0);
	border_init(shoot->border);
	vector_init(shoot->vector);
}

void shoot_set_position(shoot_t *shoot, point_t position){
	point_copy(shoot->position,&position);
}

point_t *shoot_get_position(shoot_t *shoot){
	return shoot->position;
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

border_t *shoot_get_border(shoot_t *shoot){
	return shoot->border;
}

void shoot_render(shoot_t *shoot, render_t *render){
	render->entity = shoot->type;
	render->x = point_get_x(shoot->position);
	render->y = point_get_y(shoot->position);
	render->sprite = 1;     //MOMENTANEO
	render->frame = 1;      //MOMENTANEO
}

void shoot_destroy(shoot_t **shoot){
	border_destroy((*shoot)->border);
	free((*shoot)->position);
	free((*shoot)->vector);
	free(*shoot);
	
}
