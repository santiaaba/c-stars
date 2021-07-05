#include <stdlib.h>
#include <stdint.h>
#include "../libs/point.h"
#include "border.h"
#include "vector.h"

#ifndef SHOOT_H
#define SHOOT_H

typedef struct t_shoot{
	uint32_t damage;
	point_t *position;
	border_t *border;
   vector_t *vector;
} shoot_t;

/* Inicializa el disparo */
void shoot_init(shoot_t *shoot);

/* Coloca el disparo en una coordenada específica */
void shoot_set_position(shoot_t *shoot, point_t position);

/* Retorna las coordenadas del disparo */
point_t *shoot_get_position(shoot_t *shoot);

/* Mueve el disparo */
void shoot_move(shoot_t *shoot);

border_t *shoot_get_border(shoot_t *shoot);

/* Setea el daño que ocaciona el disparo */
void shoot_set_damage(shoot_t *shoot, uint32_t damage);

/* Retorna el daño que ocaciona el disparo */
uint32_t shoot_get_damage(shoot_t *shoot);

/* Destruye la estructura del disparo */
void shoot_destroy(shoot_t **shoot);

#endif
