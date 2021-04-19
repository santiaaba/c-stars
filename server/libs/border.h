typedef struct border{
	uint32_t x,
	uint32_t y,
	uint32_t width,
	uint32_t height
} border_t

border_t *border_init(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void border_collision(border_t *border);
