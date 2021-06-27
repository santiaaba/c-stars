#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

typedef struct t_vector{
	uint32_t modulo;
	uint32_t direccion;  //Expresados en grados
} vector_t;

/* Inicializa el vector de movimiento */
void vector_init(vector_t *vector);

/* Setea el vector de movimiento */
void vector_set(vector_t *vector, uint32_t d, uint32_t m);

/* Retorna el valor de x */
int32_t vector_x(vector_t *vector);

/* Retorna el valor de y */
int32_t vector_y(vector_t *vector);

/* Copia el vector origen  en vector*/
void vector_copy(vector_t *vector, vector_t *origen);

#endif
