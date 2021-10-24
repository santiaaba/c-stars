/* Maneja un listado de bordes con coordenadas x,y
	relativ as a xx e yy */
#ifndef BORDER_H
#define BORDER_H

#include <stdlib.h>
#include <stdint.h>
#include "rect.h"
#include "../../libs/lista.h"
#include "vector.h"

typedef struct border{
	int xx;	/* Coordenada xx de origen */
	int yy;	/* Coordenada yy de origen */
	lista_t *lista;
} border_t;

/* Inicializa un borde */
void border_init(border_t *border, int xx, int yy);

void border_set_point(border_t *border, int32_t xx, int32_t yy);

/* Agrega un rectangulo a la lista */
void border_add_rect(border_t *border, rect_t *rect);

/* Determina si ha habido colicion entre los dos bordes */
uint8_t border_collision(border_t *border, border_t *border2);

/* Mueve los rectangulos que conforman el borde segun un vector */
void border_add_vector(border_t *border, vector_t *vector);

/* Dtermina si un borde esta dentro de un rectangulo llamado limit */
bool border_into_limits(border_t *border, rect_t *limit, int32_t add_x, int32_t add_y);

/* Destruye las estructuras dinamicas que conforman el borde */
void border_destroy(border_t *border);
#endif
