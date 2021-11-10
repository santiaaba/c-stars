#include <stdint.h>
#include "./point.h"
#include "./animation.h"
#include "./rect.h"
#include "../../libs/eaeapp.h"
#include "../../libs/types.h"
#include "border.h"
#include "vector.h"

#ifndef SHOOT_H
#define SHOOT_H

#define SHOOT_CREATED		1
#define SHOOT_LIVE			2	
#define SHOOT_DESTROY		3	
#define SHOOT_END				4

/* Los 20 y 30 son tipos de naves. ver types.h.
Los 0 y 10 son tipos de disparos. */
typedef struct t_shoot{
	int type;
	int state;
	uint32_t damage;
	point_t *position;
	animation_t animation;
	border_t *border;
   vector_t *vector;
	uint8_t sprite;
	uint8_t frame;
} shoot_t;

/* Inicializa el disparo */
void shoot_init(shoot_t *shoot, int type,
	int32_t x, int32_t y, float direction);

/* Coloca el disparo en una coordenada específica */
void shoot_set_position(shoot_t *shoot, int32_t x, int32_t y);

void shoot_set_animation(shoot_t *shoot, uint8_t sprite,
   uint8_t frame_size, bool loop);

/* Retorna las coordenadas del disparo */
point_t *shoot_get_position(shoot_t *shoot);

uint8_t shoot_get_state(shoot_t *shoot);
void shoot_set_state(shoot_t *shoot, uint8_t status);

/* Mueve el disparo */
void shoot_go(shoot_t *shoot);

border_t *shoot_get_border(shoot_t *shoot);

/* Retorna el daño que ocaciona el disparo */
uint32_t shoot_get_damage(shoot_t *shoot);

/* Destruye la estructura del disparo */
void shoot_destroy(shoot_t **shoot);

/* Genera los datos de la nave en el buffer */
void shoot_render(shoot_t *shoot, data_render_t *data, int16_t *sound);

#endif
