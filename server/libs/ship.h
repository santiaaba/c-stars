#ifndef SHIP_H
#define SHIP_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "border.h"
#include "border.h"
#include "shoot.h"

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

/* Agrega un rectandulo al borde de la nave. Se debe
   realizar luego del init cuando la nave esta en el punto
	(0,0). Los rectángulos deben estar definidos relativos
	a este punto.
 */
void ship_border_add(ship_t *ship, int32_t x, int32_t y, uint32_t heigh, uint32_t width);

/* Setea el vector de la nave */
void ship_set_vector(ship_t *ship, vector_t vector);

/* Determina si la nave a colicionado contra otra nave. De ser
   asi, decrementa su poder en base al poder de la nave con la que
	choca */
uint8_t ship_colision_ship(ship_t *ship, ship_t *ship2);

/* Determina si la nave a colicionado contra un disparo
   de ser así decrementa su poder en base al poder del disparo */
uint8_t ship_colision_shoot(ship_t *ship, shoot_t *shoot);

/* Retorna los bordes de una nave */
border_t ship_border(ship_t *ship);

/* Obtiene la energia de la nave */
uint32_t ship_get_power(ship_t *ship);

/* Setea la energia de la nave */
void ship_set_power(ship_t *ship, uint32_t power);

/* Retorna las coordenadas de la posicion de la nave */
point_t *ship_get_position(ship_t *ship);

#endif
