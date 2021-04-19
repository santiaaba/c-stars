#include <stdlib.h>
#include <stdint.h>
#include "point.h"

#ifndef SHOOT_H
#define SHOOT_H

typedef struct Shoot{
	uint32_t type;
	uint32_t damage;
	point_t point;
	list_t border;
	uint32_t speed;
} shoot_t

shoot_t *shoot_init(point_t point, uint32_t                                      );
point_t *shoot_get_point(shoot_t *shoot);
list_t *shoot_get_border(shoot_t *shoot);
uint32_t shoot_get_damage(shoot_t *shoot);

#endif
