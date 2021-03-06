#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

#define GRAD_0    0
#define GRAD_45   0.785398
#define GRAD_90   1.5708
#define GRAD_135  2.26893
#define GRAD_180  3.14159
#define GRAD_225  3.92699
#define GRAD_270  4.71239
#define GRAD_315  5.49779

/*
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
*/

typedef struct t_vector{
	float modulo;
	float direccion;  // Expresados en grados
} vector_t;

/* Inicializa el vector de movimiento */
void vector_init(vector_t *vector);

/* Setea el vector de movimiento */
void vector_set(vector_t *vector, float d, float m);

void vector_set_module(vector_t *vector, float m);

void vector_set_direction(vector_t *vector, float d);

float vector_get_module(vector_t *vector);

float vector_get_direction(vector_t *vector);

/* Retorna el valor de x */
int32_t vector_x(vector_t *vector);

/* Retorna el valor de y */
int32_t vector_y(vector_t *vector);

/* Copia el vector origen  en vector*/
void vector_copy(vector_t *vector, vector_t *origen);

/* Borrar vectores */
void vector_del(vector_t *vector1, vector_t *vector2);

void vector_destroy(vector_t **vector);

float vector_grad_to_rad(int grad);

#endif
