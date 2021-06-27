#include <stdlib.h>
#include <stdint.h>
#include "point.h"
#include "border.h"
#include "vector.h"

#ifndef SHOOT_H
#define SHOOT_H

typedef struct t_shoot{
	uint32_t damage;
	point_t point;
	border_t border;
   vector_t vector;
} shoot_t

/* Inicializa el disparo */
void shoot_init(shoot_t *shoot);

/* Coloca el disparo en una coordenada específica */
void shoot_set_position(shoot_t *shoot, point_t p);

/* Mueve el disparo cambiando sus coordenadas en
   base a su vector */
void shoot_move(shoot_t *shoot);

/* Retorna las coordenadas del disparo */
point_t shoot_get_position(shoot_t *shoot);

/* Setea el daño que ocaciona el disparo */
void shoot_set_damage(shoot_t *shoot, uint32_t damage);

/* Retorna el daño que ocaciona el disparo */
uint32_t shoot_get_damage(shoot_t *shoot);

#endif
