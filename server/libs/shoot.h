#include <stdint.h>
#include "./point.h"
#include "../../libs/eaeapp.h"
#include "border.h"
#include "vector.h"

#ifndef SHOOT_H
#define SHOOT_H

/* Los 20 y 30 son tipos de naves. ver ship.h.
Los 0 y 10 son tipos de disparos. */
#define SHOOT_1	0
#define SHOOT_2	1
#define SHOOT_3	2
#define SHOOT_4	3
#define SHOOT_5	4

typedef struct t_shoot{
	int type;
	uint32_t damage;
	point_t *position;
	border_t *border;
   vector_t *vector;
	uint8_t sprite;
	uint8_t frame;
} shoot_t;

/* Inicializa el disparo */
void shoot_init(shoot_t *shoot, int type);

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
void shoot_destroy(void **shoot);

/* Genera los datos de la nave en el buffer */
void shoot_render(shoot_t *shoot, data_render_t *render);

#endif
