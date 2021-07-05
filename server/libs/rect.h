#include <stdlib.h>
#include <stdint.h>
#include "../libs/point.h"

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
void rect_set_point(rect_t *rect, int32_t x, int32_t y);

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

/* No hace nada. Ya que un rect no esta formado por punteros */
void rect_destroy(rect_t **rect);

#endif
