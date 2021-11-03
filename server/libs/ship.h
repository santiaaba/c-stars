#ifndef SHIP_H
#define SHIP_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include "vector.h"
#include "../../libs/eaeapp.h"
#include "../../libs/types.h"
#include "border.h"
#include "rect.h"
#include "weapon.h"
#include "shoot.h"
#include "clockgame.h"
#include "animation.h"

/* Los 20 y 30 son tipos de naves.
Los 0 y 10 son tipos de disparos. Ver shoot.h */

/* Estados de la nave */
#define	SHIP_LIVE		0	/* La nave vive */
#define	SHIP_DESTROY	1	/* La nave se esta destruyendo */
#define	SHIP_END			2	/* La nave deja de existir */

typedef struct t_ia_mov{
	vector_t vector;
	uint16_t instant;
} ia_mov_t;

typedef struct t_ia{
	lista_t *path;
	uint16_t time_start;	/* Instante de tiempo en que
									se inicio el IA */
	clockgame_t *clock;
} ia_t;

typedef struct t_ship{
	int power;
	point_t *position;
	uint8_t state;
	rect_t limits;
	bool limited;
	bool tangible;
	border_t *border;
	weapon_t weapon;
	vector_t *vector;
	uint32_t addx;		/* Incremento movimiento */
	uint32_t addy;		/* Incremento movimiento */
	animation_t animation;
//	float speed;
	uint8_t type;		// ID tipo nave
	ia_t *ia;
	uint8_t ia_activated;
} ship_t;

/* Inicializa una nave */
void ship_init(ship_t *ship, uint8_t type, clockgame_t* clockagame, lista_t *shoots);

/* Permite colocar la nave en coordenadas especificas */
void ship_set_position(ship_t *ship, int32_t x, int32_t y);

/* Setea la velocidad de la nave */
void ship_set_speed(ship_t *ship, float speed);

void ship_set_tangible(ship_t *ship, bool tangible);

bool ship_is_tangible(ship_t *ship);

int ship_get_type(ship_t *ship);

void ship_begin_destroy(ship_t *ship);

/* Retorna la velocidad de la nave */
float ship_get_speed(ship_t *ship);

void ship_shooting(ship_t *ship, bool on);

bool ship_is_shooting(ship_t *ship);

/* Setea la direccion de la nave */
void ship_set_direction(ship_t *ship, float direction);

/* Retorna la direccion de la nave */
float ship_get_direction(ship_t *ship);

/* Setea los limites inferior y derecho. El superior
   y el izquierdo son 0 y 0 respectivamente */
void ship_set_limits(ship_t *ship,int limit_bottom, int limit_right);

/* Remueve los limites de la nave en sus movimientos */
void ship_remove_limits(ship_t *ship);

/* Mueve la nave. Ejecuta la instaucción de la ia
	si esta esta activada. Anima la nave */
void ship_go(ship_t *ship);

/* Agrega un rectandulo al borde de la nave. Se debe
   realizar luego del init cuando la nave esta en el punto
	(0,0). Los rectángulos deben estar definidos relativos
	a este punto.
 */
void ship_border_add(ship_t *ship, int32_t x, int32_t y, uint32_t heigh, uint32_t width);

/* Para el manejo de los estados */
void ship_set_state(ship_t *ship, uint8_t state);
uint8_t ship_get_state(ship_t *ship);

/* Setea el vector de la nave */
void ship_set_vector(ship_t *ship, vector_t *vector);

vector_t *ship_get_vector(ship_t *ship);

/* Determina si la nave a colicionado contra otra nave. De ser
   asi, decrementa su poder en base al poder de la nave con la que
	choca */
uint8_t ship_colision_ship(ship_t *ship, ship_t *ship2);

/* Determina si la nave a colicionado contra un disparo
   de ser así decrementa su poder en base al poder del disparo */
uint8_t ship_colision_shoot(ship_t *ship, shoot_t *shoot);

/* Retorna los bordes de una nave */
border_t *ship_border(ship_t *ship);

/* Obtiene la energia de la nave */
int ship_get_power(ship_t *ship);

/* Setea una animacion */
void ship_set_animation(ship_t *ship, uint8_t sprite, uint8_t frame_size, bool loop);

/* Setea la energia de la nave */
void ship_set_power(ship_t *ship, int power);

/* Retorna las coordenadas de la posicion de la nave */
point_t *ship_get_position(ship_t *ship);

/* Retorna el ia de la nave */
ia_t *ship_get_ia(ship_t *ship);

/* Activa la ia de la nave */
void ship_ia_activate(ship_t *ship);

/* Destruye una nave */
void ship_destroy(ship_t *ship);

/* Genera los datos de la nave en el buffer */
void ship_render(ship_t *ship, data_render_t *data);

/****************************
			Para la IA
*****************************/

/* Inicializa la estructura de la ia */
void ia_init(ia_t *ia, clockgame_t *clock);

/* Inicia la ia */
void ia_start(ia_t *ia);

/* Maneja la nave cambiando el vector de movimiento
	si correspondiese */
void ia_drive_ship(ia_t *ia, ship_t *ship);

void ia_add_path(ia_t *ia, uint16_t instant,uint32_t direction, uint32_t speed);

void ia_destroy(ia_t *ia);

#endif
