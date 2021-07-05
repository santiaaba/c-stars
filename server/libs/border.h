#include <stdlib.h>
#include <stdint.h>
#include "rect.h"
#include "../libs/lista.h"
#include "vector.h"

#ifndef BORDER_H
#define BORDER_H

typedef struct border{
	lista_t *lista;
} border_t;

/* Inicializa un borde */
void border_init(border_t *border);

/* Agrega un rectangulo a la lista */
void border_add_rect(border_t *border, rect_t *rect);

/* Determina si ha habido colicion entre los dos bordes */
uint8_t border_collision(border_t *border, border_t *border2);

/* Mueve los rectangulos que conforman el borde segun un vector */
void border_add_vector(border_t *border, vector_t *vector);

/* Destruye las estructuras dinamicas que conforman el borde */
void border_destroy(border_t **border);
#endif
