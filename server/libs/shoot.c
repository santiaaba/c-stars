#include "shoot.h"

void shoot_init(shoot_t *shoot, int type){
	shoot->type = type;
	shoot->damage = 0;
	point_set(shoot->position,0,0);
	border_init(shoot->border,0,0);
	vector_init(shoot->vector);
	shoot->sprite = 0;
	shoot->frame = 0;
}

void shoot_set_position(shoot_t *shoot, int32_t x, int32_t y){
	point_set(shoot->position,x,y);
	border_set_point(shoot->border,x,y);
}

point_t *shoot_get_position(shoot_t *shoot){
	return shoot->position;
}

void shoot_move(shoot_t *shoot){
	point_add_vector(shoot->position,shoot->vector);
	border_set_point(shoot->border,shoot->position->x,
		shoot->position->y);
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

void shoot_render(shoot_t *shoot, data_render_t *data){
	data->entity_class = shoot->type;
	data->pos_x = point_get_x(shoot->position);
	data->pos_y = point_get_y(shoot->position);
	data->sprite = shoot->sprite;
	data->frame = shoot->frame;
}

void shoot_destroy(void **shoot){
	border_destroy((void *)(&(((shoot_t*)(*shoot))->border)));
	free(((shoot_t*)(*shoot))->position);
	free(((shoot_t*)(*shoot))->vector);
	free(*shoot);
}
