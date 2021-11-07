#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef RECT_H
#define RECT_H

typedef struct rect{
	int x;
	int y;
	uint16_t width;
	uint16_t height;
} rect_t;

/* Inicializa un rectangulo */
void rect_init(rect_t *rect);

/* Setea el punto del rectangulo */
void rect_set_point(rect_t *rect, int x, int y);

void rect_get_point(rect_t *rect, int *x, int *y);

/* Setea las dimenciones del rectangulo */
void rect_set_dim(rect_t *rect, uint16_t width, uint16_t height);

/* Retorna el ancho del rectangulo */
uint32_t rect_get_width(rect_t *rect);

/* Retorna el alto del rectangulo */
uint32_t rect_get_height(rect_t *rect);

/* Determina si ha habido colicion entre el rectangulo y un segundo
   rectangulo pasado por parametro */
uint16_t rect_collision(rect_t *rect, rect_t *rect2);

bool rect_into_rect(rect_t *limit, rect_t *rect);
bool rect_out_rect(rect_t *limit, rect_t *rect);

/* Copia los datos del rec origen en destino */
void rect_copy(rect_t *dest, rect_t *origen);

/* No hace nada. Ya que un rect no esta formado por punteros */
void rect_destroy(rect_t **rect);

#endif
