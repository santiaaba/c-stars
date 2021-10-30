#include "rect.h"

void rect_init(rect_t *rect){
	rect->x = 0;
	rect->y = 0;
	rect->width = 0;
	rect->height = 0;
}

void rect_get_point(rect_t *rect, int32_t *x, int32_t *y){
	*x = rect->x;
	*y = rect->y;
}

void rect_set_point(rect_t *rect, int32_t x, int32_t y){
	rect->x = x;
	rect->y = y;
}

void rect_set_dim(rect_t *rect, uint32_t width, uint32_t height){
	rect->width = width;
	rect->height = height;
}

uint32_t rect_get_width(rect_t *rect){
	return rect->width;
}

uint32_t rect_get_height(rect_t *rect){
	return rect->height;
}

bool rect_into_rect(rect_t *limit, rect_t *rect){
	return (	rect->y > limit->y &&
				rect->y + rect->height < limit->y + limit->height &&
			 	rect->x > limit->x &&
				rect->x + rect->width < limit->x + limit->width);
}

void rect_copy(rect_t *dest, rect_t *origen){
	dest->x = origen->x;
	dest->y = origen->y;
	dest->height = origen->height;
	dest->width = origen->width;
}

uint16_t rect_collision(rect_t *rect, rect_t *rect2){
	
	bool crash;
	int32_t b_x = rect->x;
	int32_t b_y = rect->y;
	uint32_t b_h = rect->height;
	uint32_t b_w = rect->width;
	int32_t o_x = rect2->x;
	int32_t o_y = rect2->y;
	uint32_t o_h = rect2->height;
	uint32_t o_w = rect2->width;

	crash = (o_x > b_x && o_x < b_x + b_w &&
	((o_y < b_y && o_y > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h))) ||

	(o_x + o_w > b_x && o_x < b_x + b_w &&
	((o_y + o_w < b_y && o_y + o_w > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h)));

//	printf("rect_collision:(x,y,w,h) =>(%i,%i,%i,%i) | (%i,%i,%i,%i) | %i\n",
//			b_x,b_y,b_w,b_h,o_x,o_y,o_w,o_h,crash);

	return crash;
}

void rect_destroy(rect_t *rect){
}
