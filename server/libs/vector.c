#include "vector.h"

void vector_init(vector_t *vector){
	vector->direccion = 0;
	vector->modulo = 0;
}

void vector_set(vector_t *vector, float d, float m){
	vector->direccion = d;
	vector->modulo = m;
}

void vector_set_module(vector_t *vector, float m){
	vector->modulo = m;
}

void vector_set_direction(vector_t *vector, float d){
	vector->direccion = d;
}

float vector_get_module(vector_t *vector){
	return vector->modulo;
}

float vector_get_direction(vector_t *vector){
	return vector->direccion;
}

int32_t vector_x(vector_t *vector){
	return (int32_t)(vector->modulo * cos(vector->direccion));
}

int32_t vector_y(vector_t *vector){
	/* Le debemos cambiar el signo ya que graficamente
	   el eje coordenado y invertido */
	return -(int32_t)(vector->modulo * sin(vector->direccion));
}

void vector_copy(vector_t *vector, vector_t *origen){
	vector->direccion = origen->direccion;
	vector->modulo = origen->modulo;
}

float vector_grad_to_rad(int grad){
	return grad * (3.141592/180);
}

void vector_del(vector_t *vector1, vector_t *vector2){
}

void vector_destroy(vector_t **vector){
	/* Permite destruir un vector cuando es
		un puntero */
	free(*vector);
}
