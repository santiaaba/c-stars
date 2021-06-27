#include "vector.h"

void vector_init(vector_t *vector){
	vector->direccion = 0;
	vector->modulo = 0;
}

void vector_set(vector_t *vector, uint32_t d, uint32_t m){
	vector->direccion = d;
	vector->modulo = m;
}

int32_t vector_x(vector_t *vector){
	return vector->modulo * cos(vector->direccion);
}

int32_t vector_y(vector_t *vector){
	return vector->modulo * sin(vector->direccion);
}

void vector_copy(vector_t *vector, vector_t *origen){
	vector->direccion = origen->direccion;
	vector->modulo = origen->modulo;
}
