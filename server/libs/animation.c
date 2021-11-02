#include "animation.h"

void animation_init(animation_t *a, uint8_t sprite, uint8_t frame_size, bool loop){
	a->sprite = sprite;
	a->frame_size = frame_size;
	a->frame = 0;
	a->loop = loop;
}

void animation_get(animation_t *a, uint8_t *sprite, uint8_t *frame){
	*sprite = a->sprite;
	*frame = a->frame;
}

void animation_next(animation_t *a){
	if(a->frame < a->frame_size - 1)
		a->frame++;
	else
		if(a->loop)
			a->frame=0;
}

bool animation_end(animation_t *a){
	return (a->frame_size - 1) == a->frame;
}
