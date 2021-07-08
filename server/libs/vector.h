#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef enum {
   TOP_PRESS,
   TOP_RELEASE,
   BOTTOM_PRESS,
   BOTTOM_RELEASE,
   LEFT_PRESS,
   LEFT_RELEASE,
   RIGHT_PRESS,
   RIGHT_RELEASE,
   FIRE_PRESS,
   FIRE_RELEASE,
   PAUSE_PRESS,
   START_PRESS
} direction_t;

typedef struct t_vector{
	float modulo;
	uint32_t direccion;  // Expresados en grados
} vector_t;

/* Inicializa el vector de movimiento */
void vector_init(vector_t *vector);

/* Setea el vector de movimiento */
void vector_set(vector_t *vector, uint32_t d, float m);

/* Retorna el valor de x */
int32_t vector_x(vector_t *vector);

/* Retorna el valor de y */
int32_t vector_y(vector_t *vector);

/* Copia el vector origen  en vector*/
void vector_copy(vector_t *vector, vector_t *origen);

#endif
