#ifndef SHIP_H
#define SHIP_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "border.h"

typedef enum ship_type { HEROE, ZANGANO, ZANGANO2, BOMBARDERO, DESTRUCTOR1, DESTRUCTOR2, JEFE } ship_type_t;

typedef struct t_ship{
	uint32_t power;
	point_t *position;
	border_t *border;
	vector_t *vector;
	ship_type_t type;
} ship_t;

/* Inicializa una nave */
void ship_init(ship_t *ship, ship_type_t type);

/* Permite colocar la nave en coordenadas especificas */
void ship_set_position(ship_t *ship, point_t *point);

/* Mueve la nave */
void ship_move(ship_t *ship);

/* Setea el vector de la nave */
void ship_set_vector(ship_t *ship, vector_t vector);

/* Determina si la nave a colicionado contra algun borde de la lista */
uint8_t ship_colision(ship_t *ship, border_t *border);

/* Obtiene la energia de la nave */
uint32_t ship_get_power(ship_t *ship);

/* Setea la energia de la nave */
void ship_set_power(ship_t *ship, uint32_t power);

/* Retorna las coordenadas de la posicion de la nave */
point_t *ship_get_position(ship_t *ship);

#endif
