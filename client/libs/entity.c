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
