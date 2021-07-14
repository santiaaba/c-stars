#include "entity.h"

void entity_init(entity_t *entity, SDL_Texture *texture, uint16_t type){
	entity->texture = texture;
	switch(type){
		case 0:
			(entity->frame).x = 0;		(entity->position).x = 0;
			(entity->frame).y = 0;		(entity->position).y = 0;
			(entity->frame).w = 64;		(entity->position).w = 64;
			(entity->frame).h = 87;		(entity->position).h = 87;
			break;
	}
}

void entity_set_position(entity_t *entity, int32_t x, int32_t y){
	(entity->position).x = x;
	(entity->position).y = y;
}

void entity_set_frame(entity_t *entity, uint16_t row, uint16_t col){
	(entity->frame).x = (entity->frame).h * row;
	(entity->frame).y = (entity->frame).w * col;
}

void entity_copy(entity_t *dest, entity_t *src){
	dest->frame.x = src->frame.x;
	dest->frame.y = src->frame.y;
	dest->frame.w = src->frame.w;
	dest->frame.h = src->frame.h;

	dest->position.x = src->position.x;
	dest->position.y = src->position.y;
	dest->position.w = src->position.w;
	dest->position.h = src->position.h;

	dest->texture = src->texture;
}

void entity_draw(entity_t *entity, SDL_Renderer *renderer){
	SDL_RenderCopy(
		renderer,
		entity->texture,
		&(entity->frame),
		&(entity->position));
}

void entity_destroy(entity_t **entity){
	free(*entity);
}
