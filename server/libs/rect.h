#include <stdlib.h>
#include <stdint.h>
#include "point.h"

#ifndef RECT_H
#define RECT_H

typedef struct rect{
	point_t *point;
	uint32_t width;
	uint32_t height;
} rect_t;

/* Inicializa un rectangulo */
void rect_init(rect_t *rect);

/* Setea el punto del rectangulo */
void rect_set_point(rect_t *rect, point_t *p);

point_t *rect_get_point(rect_t *rect);

/* Setea las dimenciones del rectangulo */
void rect_set_dim(rect_t *rect, uint32_t width, uint32_t height);

/* Retorna el ancho del rectangulo */
uint32_t rect_get_width(rect_t *rect);

/* Retorna el alto del rectangulo */
uint32_t rect_get_height(rect_t *rect);

/* Determina si ha habido colicion entre el rectangulo y un segundo
   rectangulo pasado por parametro */
uint16_t rect_collision(rect_t *rect, rect_t *rect2);

#endif
