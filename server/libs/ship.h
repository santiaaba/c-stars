#ifndef SHIP_H
#define SHIP_H

#include <stdio.h>
#include <stdlib.h>
#include "vector.h"
#include "border.h"
#include "rect.h"
#include "shoot.h"
#include "render.h"
#include "clockgame.h"

#define PLAYER			0
#define ENEMIE1		1
#define ENEMIE2		2
#define ENEMIE3		3
#define ENEMIE4		4

typedef struct t_ia_mov{
	vector_t *vector;
	uint32_t duration;	/* Duracion en segundos de este movimiento */
} ia_mov_t;

typedef struct t_ia{
	lista_t *path;
	uint32_t time_last_mov;	/* Instante de tiempo en que
										se inicio el ultimo movimiento */
	clockgame_t *clock;
} ia_t;

typedef struct t_ship{
	uint32_t power;
	point_t *position;
	border_t *border;
	vector_t *vector;
	float speed;
	uint8_t type;		// ID tipo nave
	ia_t *ia;
	uint8_t ia_activated;
} ship_t;

/* Inicializa una nave */
void ship_init(ship_t *ship, uint8_t type, clockgame_t* clockagame);

/* Permite colocar la nave en coordenadas especificas */
void ship_set_position(ship_t *ship, int32_t x, int32_t y);

/* Setea la velocidad de la nave */
void ship_set_speed(ship_t *ship, float speed);

/* Retorna la velocidad de la nave */
float ship_get_speed(ship_t *ship);

/* Mueve la nave. Ejecuta la instaucción de la ia
	si esta esta activada */
void ship_move(ship_t *ship);

/* Agrega un rectandulo al borde de la nave. Se debe
   realizar luego del init cuando la nave esta en el punto
	(0,0). Los rectángulos deben estar definidos relativos
	a este punto.
 */
void ship_border_add(ship_t *ship, int32_t x, int32_t y, uint32_t heigh, uint32_t width);

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
uint32_t ship_get_power(ship_t *ship);

/* Setea la energia de la nave */
void ship_set_power(ship_t *ship, uint32_t power);

/* Retorna las coordenadas de la posicion de la nave */
point_t *ship_get_position(ship_t *ship);

/* Retorna el ia de la nave */
ia_t *ship_get_ia(ship_t *ship);

/* Activa la ia de la nave */
void ship_ia_activate(ship_t *ship);

/* Destruye una nave */
void ship_destroy(ship_t **ship);

/* Genera los datos de la nave en el buffer */
void ship_render(ship_t *ship, render_t *render);

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

void ia_add_path(ia_t *ia, uint32_t instant,uint32_t direction, uint32_t speed);

void ia_destroy(ia_t **ia);

#endif
