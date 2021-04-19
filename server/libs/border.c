include "border.h";

border_t *border_init(point_t point, uint32_t width, uint32_t height){
	border_t *border ;
	border = border_t *malloc(typedef(border_t));
	border->point = point
	border->width = width;
	border->height = height;
	return border;
}

void border_collision(border_t *border, border_t *other){
	
	uint32_t b_x = point_get_x(border->point)
	uint32_t b_y = point_get_y(border->point)
	uint32_t b_h = border->height
	uint32_t b_w = border->width

	uint32_t o_x = point_get_x(border_get_point(other))
	uint32_t o_y = point_get_y(border_get_point(other))
	uint32_t o_h = border_get_height(other)
	uint32_t o_w = border_get_width(other)

	return (o_x > b_x && o_x < b_x + b_w &&
	((o_y < b_y && o_y > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h))) ||

	(o_x + o_w > b_x && o_x < b_x + b_w &&
	((o_y + o_w < b_y && o_y + o_w > b_y - b_h) ||
	(o_y + o_h < b_y && o_y + o_h > b_y - b_h)))
}
