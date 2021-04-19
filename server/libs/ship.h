typedef struct t-ship{
	uint32_t power;
	point_t position;
	list_t border;
} ship_t

void ship_init(	ship_t *ship,
						poin_t *position,
						list_t *borders);
void ship_change_weapon();
void ship_colision(ship_t *ship, void *list_t *borders)
void ship_get_power(ship_t *ship)
void ship_get_position(ship_t *ship)
void ship_get_border(ship_t *ship)

void ship_set_power(ship_t *ship)
void ship_shoot(ship_t *ship, list_t *shots)
