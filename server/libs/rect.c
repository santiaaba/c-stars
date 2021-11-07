#include "rect.h"

void rect_init(rect_t *rect){
	rect->x = 0;
	rect->y = 0;
	rect->width = 0;
	rect->height = 0;
}

void rect_get_point(rect_t *rect, int *x, int *y){
	*x = rect->x;
	*y = rect->y;
}

void rect_set_point(rect_t *rect, int x, int y){
	rect->x = x;
	rect->y = y;
}

void rect_set_dim(rect_t *rect, uint16_t width, uint16_t height){
	rect->width = width;
	rect->height = height;
}

uint rect_get_width(rect_t *rect){
	return rect->width;
}

uint rect_get_height(rect_t *rect){
	return rect->height;
}

bool rect_into_rect(rect_t *limit, rect_t *rect){
	return (	rect->y > limit->y &&
				rect->y + rect->height < limit->y + limit->height &&
			 	rect->x > limit->x &&
				rect->x + rect->width < limit->x + limit->width);
}

bool rect_out_rect(rect_t *limit, rect_t *rect){

	bool out;
	out = (	(rect->x + rect->width) < limit->x ||
				(limit->x + limit->width) < rect->x ||
				(rect->y + rect->height) < limit->y ||
				(limit->y + limit->height) < rect->y);
/*
	printf("rect_out?:(x,y,w,h) =>(%i,%i,%i,%i) | (%i,%i,%i,%i) | %i,%i,%i,%i\n",
			rect->x,rect->y,rect->width,rect->height,
			limit->x,limit->y,limit->width,limit->height,
			(rect->x + rect->width) < limit->x,
			(limit->x + limit->width) < rect->x,
			(rect->y + rect->height) < limit->y,
			(limit->y + limit->height) < rect->y
			);
*/
	return out;
}

void rect_copy(rect_t *dest, rect_t *origen){
	dest->x = origen->x;
	dest->y = origen->y;
	dest->height = origen->height;
	dest->width = origen->width;
}

uint16_t rect_collision(rect_t *rect, rect_t *rect2){
	
	bool crash;
	int b_x = rect->x;
	int b_y = rect->y;
	uint b_h = rect->height;
	uint b_w = rect->width;
	int o_x = rect2->x;
	int o_y = rect2->y;
	uint o_h = rect2->height;
	uint o_w = rect2->width;

	crash = ((o_x > b_x && o_x < b_x + b_w) ||
		(o_x + o_w > b_x && o_x + o_w < b_x + b_w)) &&
		((o_y + o_h > b_y && o_y + o_h < b_y + b_h) ||
		 (o_y > b_y && o_y < b_y + b_h));

/*
	crash = (o_x > b_x && o_x < b_x + b_w &&
		(
			(o_y < b_y && o_y > b_y - b_h) ||
			(o_y + o_h < b_y && o_y + o_h > b_y - b_h)
		)
	)
	||
		(o_x + o_w > b_x && o_x < b_x + b_w &&
	((o_y + o_w < b_y && o_y + o_w > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h)));
*/
/*	crash =	(o_x + o_w > b_x && o_y + o_h > b_y) ||
				(o_x < b_x + b_w && o_y + o_h > b_y) ||
				(o_x + o_w > b_x && o_y < b_y + b_h) ||
				(o_x < b_x + b_w && o_y < b_y + b_h); */

//	printf("rect_collision:(x,y,w,h) =>(%i,%i,%i,%i) | (%i,%i,%i,%i) | %i\n",
//			b_x,b_y,b_w,b_h,o_x,o_y,o_w,o_h,crash);

	return crash;
}

void rect_destroy(rect_t **rect){
	/* Destruye un rect cuando es un puntero*/
	free(*rect);
}
