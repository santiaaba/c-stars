#ifndef ANIMATION_H
#define ANIMATION_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	uint8_t sprite;			/* ID del sprite */
	uint8_t frame_size;		/* Cantidad de frames */
	uint8_t frame;				/* Frame actual */
	bool loop;					/* Se repite? */
} animation_t;

void animation_init(animation_t *a, uint8_t sprite, uint8_t frame_size, bool loop);
void animation_get(animation_t *a, uint8_t *sprite, uint8_t *frame);
void animation_next(animation_t *a);
bool animation_end(animation_t *a);
#endif
