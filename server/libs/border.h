/* Maneja un listado de bordes con coordenadas x,y
	relativ as a xx e yy */
#ifndef BORDER_H
#define BORDER_H

#include <stdlib.h>
#include <stdint.h>
#include "rect.h"
#include "../../libs/lista.h"
#include "point.h"

typedef struct border{
	point_t *position;
	int xx;	/* Coordenada xx de origen */
	int yy;	/* Coordenada yy de origen */
	lista_t *lista;
	rect_t big;		/* El es borde que engloba al elemento */
} border_t;

/* Inicializa un borde */
void border_init(border_t *border, point_t *position, int w, int h);

/* Agrega un rectangulo a la lista */
void border_add_rect(border_t *border, rect_t *rect);

/* Determina si ha habido colicion entre los dos bordes */
uint8_t border_collision(border_t *border, border_t *border2);

/* Dtermina si un borde esta dentro de un rectangulo llamado limit */
bool border_into_limits(border_t *border, rect_t *limit, int32_t add_x, int32_t add_y);

bool border_out_limits(border_t *border, rect_t *limit);

/* Destruye las estructuras dinamicas que conforman el borde */
void border_destroy(border_t **border);
#endif
