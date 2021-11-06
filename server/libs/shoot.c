#include "shoot.h"

void shoot_init(shoot_t *shoot, int type,
	int32_t x, int32_t y, float direction){

	rect_t *create_rect(int32_t x, int32_t y, uint32_t width, uint32_t height){
		rect_t *newRect;
		newRect = (rect_t*)malloc(sizeof(rect_t));
		rect_init(newRect);
		rect_set_dim(newRect,width,height);
		rect_set_point(newRect,x,y);
		return newRect;
   }

	rect_t *rect;

	shoot->position=(point_t*)malloc(sizeof(point_t));
	shoot->border=(border_t*)malloc(sizeof(border_t));
   shoot->vector=(vector_t*)malloc(sizeof(vector_t));

	shoot->type = type;
	point_set(shoot->position,0,0);
	vector_init(shoot->vector);
	shoot->sprite = 0;
	shoot->frame = 0;
	point_set(shoot->position,x,y);
	//border_set_point(shoot->border,x,y);

	shoot->state = SHOOT_LIVE;

	switch(type){
		case SHOOT_1:
			shoot->damage = 10;
			vector_set(shoot->vector,direction,15);
			border_init(shoot->border,shoot->position,10,10);
			rect = create_rect(0,0,10,10);
         border_add_rect(shoot->border,rect);
			shoot_set_animation(shoot,0,1,false);
			break;
		default:
			printf("shoot_init(): ERROR FATAL: no existe el tipo de shoot\n");
	}
}

void shoot_set_animation(shoot_t *shoot, uint8_t sprite,
   uint8_t frame_size, bool loop){
   animation_init(&(shoot->animation),sprite,frame_size, loop);
}

uint8_t shoot_get_state(shoot_t *shoot){
	return shoot->state;
}

void shoot_set_state(shoot_t *shoot, uint8_t state){
	shoot->state = state;
}

void shoot_set_position(shoot_t *shoot, int32_t x, int32_t y){
	point_set(shoot->position,x,y);
	//border_set_point(shoot->border,x,y);
}

point_t *shoot_get_position(shoot_t *shoot){
	return shoot->position;
}

void shoot_go(shoot_t *shoot){
	animation_next(&(shoot->animation));
	point_add_vector(shoot->position,shoot->vector);
	//border_set_point(shoot->border,shoot->position->x,
	//	shoot->position->y);
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
	animation_get(&(shoot->animation),&(data->sprite),&(data->frame));
}

void shoot_destroy(shoot_t **shoot){
	point_destroy(&((*shoot)->position));
	border_destroy(&((*shoot)->border));
	vector_destroy(&((*shoot)->vector));

	free(*shoot);
}
