#include "object.h"

void object_init(object_t *object){
	object->border
	object->vector
	object->damage = 0;
}

void object_set_position(object_t *object, point_t p){
}

void object_add_vector(object_t *object, vector_t *vector){
}

int32_t object_get_x(object_t *object){
}

int32_t object_get_y(object_t *object){
}

void object_set_damage(object_t *object, uint32_t damage){
}

uint32_t object_get_damage(object_t *object){
}
