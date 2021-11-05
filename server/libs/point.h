#ifndef POINT_H
#define POINT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "vector.h"

typedef struct Point {
	int32_t x;
	int32_t y;
} point_t;

/* Inicializa unpunto */
void point_init(point_t *point);

/* Coloca coordenadas especificas para el punto */
void point_set(point_t *point, int32_t x, int32_t y);

/* Cambia las coordenadas del punto sumando un vector */
void point_add_vector(point_t *point, vector_t *vector);

void point_add(point_t *point, int x, int y);

/* Retorna las coordenadas en x */
int32_t point_get_x(point_t *point);

/* Retorna las coordenadas en y */
int32_t point_get_y(point_t *point);

/* Copia las coordenadas de other en point */
void point_copy(point_t *point, point_t *other);

void point_destroy(point_t **point);
#endif
