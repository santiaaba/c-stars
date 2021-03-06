#include "ship.h"

void ship_init(ship_t *ship, uint8_t type, clockgame_t *clock, lista_t *shoots){

	rect_t *create_rect(int32_t x, int32_t y, uint32_t width, uint32_t height){
		rect_t *newRect;
		newRect = (rect_t*)malloc(sizeof(rect_t));
		rect_init(newRect);
		rect_set_dim(newRect,width,height);
		rect_set_point(newRect,x,y);
		return newRect;
	}

	rect_t *rect;

	rect_init(&(ship->limits));
	rect_set_point(&(ship->limits),0,0);
	rect_set_dim(&(ship->limits),0,0);

	ship->position=(point_t*)malloc(sizeof(point_t));
	ship->border=(border_t*)malloc(sizeof(border_t));
	ship->vector=(vector_t*)malloc(sizeof(vector_t));
	ship->ia=(ia_t*)malloc(sizeof(ia_t));

	point_init(ship->position);
	vector_init(ship->vector);
	ia_init(ship->ia, clock);

	ship->tangible = true;
	ship->type = type;
	ship->ia_activated = 0;
	ship->state = SHIP_LIVE;
	ship->addx = 0;
	ship->addy = 0;

	/* Asignamos los bordes y las animaciones de inicio */
	switch(type){
		case SHIP_PLAYER:
			ship -> power = 100;

			border_init(ship->border,ship->position,64,88);
			rect = create_rect(54,34,15,18);
			border_add_rect(ship->border,rect);
			rect = create_rect(52,73,13,17);
			border_add_rect(ship->border,rect);
			rect = create_rect(4,35,55,23);
			border_add_rect(ship->border,rect);
			rect = create_rect(20,3,19,30);
			border_add_rect(ship->border,rect);
			rect = create_rect(20,55,19,30);
			border_add_rect(ship->border,rect);
			
			animation_init(&(ship->animation),0,1,false);
			ship->limited = true;
			weapon_init(&(ship->weapon),WEAPON_1,2,clock,
				GRAD_0,false,100,ship->position,62,36,shoots);
			break;
		case SHIP_ENEMIE1:
			ship -> power = 30;
			border_init(ship->border,ship->position,133,138);

			rect = create_rect(19,57,105,26);
			border_add_rect(ship->border,rect);

			rect = create_rect(45,26,20,32);
			border_add_rect(ship->border,rect);

			rect = create_rect(45,84,20,32);
			border_add_rect(ship->border,rect);

			rect = create_rect(62,10,44,118);
			border_add_rect(ship->border,rect);

			animation_init(&(ship->animation),0,1,false);
			ship->limited = false;
			weapon_init(&(ship->weapon),WEAPON_1,20,clock,
				GRAD_180,true,30,ship->position,0,69,shoots);
			break;
		case SHIP_ENEMIE2:
			ship -> power = 100;
			border_init(ship->border,ship->position,161,119);

			rect = create_rect(3,6,148,25);
			border_add_rect(ship->border,rect);

			rect = create_rect(5,87,148,25);
			border_add_rect(ship->border,rect);

			rect = create_rect(21,38,124,40);
			border_add_rect(ship->border,rect);

			animation_init(&(ship->animation),0,1,false);
			ship->limited = false;
			weapon_init(&(ship->weapon),WEAPON_1,20,clock,
				GRAD_180,true,30,ship->position,0,69,shoots);
			break;

	}
}

void ship_begin_destroy(ship_t *ship){
	ship->state = SHIP_DESTROY;
	ship->tangible = false;
	ship_set_animation(ship,1,15,false);
	weapon_shooting(&(ship->weapon),false);
}

bool ship_is_shooting(ship_t *ship){
	return weapon_is_shooting(&(ship->weapon));
}

void ship_set_tangible(ship_t *ship, bool tangible){
	ship->tangible = tangible;
}

bool ship_is_tangible(ship_t *ship){
	return ship->tangible;
}

void ship_shooting(ship_t *ship, bool on){
	weapon_shooting(&(ship->weapon),on);
}

void ship_set_limits(ship_t *ship,int limit_bottom, int limit_right){
	rect_set_dim(&(ship->limits),limit_right,limit_bottom);
}

void ship_activate_limits(ship_t *ship){
	ship->limited = true;
}

void ship_remove_limits(ship_t *ship){
	ship->limited = false;
}

void ship_set_state(ship_t *ship, uint8_t state){
	ship->state = state;
}

void ship_reset_weapon(ship_t *ship){
	weapon_reset(&(ship->weapon));
}

uint8_t ship_get_state(ship_t *ship){
	return ship->state;
}

void ship_set_speed(ship_t *ship, float speed){
	vector_set_module(ship->vector,speed);
	ship->addx = vector_x(ship->vector);
	ship->addy = vector_y(ship->vector);
}

float ship_get_speed(ship_t *ship){
	return vector_get_module(ship->vector);
}

void ship_set_direction(ship_t *ship, float direction){
	vector_set_direction(ship->vector,direction);
	ship->addx = vector_x(ship->vector);
	ship->addy = vector_y(ship->vector);
}

float ship_get_direction(ship_t *ship){
	return vector_get_direction(ship->vector);
}

int ship_get_type(ship_t *ship){
	return ship->type;
}

void ship_set_position(ship_t *ship, int32_t x, int32_t y){
	point_set(ship->position,x,y);
	//border_set_point(ship->border,x,y);
}

void ship_go(ship_t *ship){

	if(ship -> ia_activated && ship->state == SHIP_LIVE){
		ia_drive_ship(ship->ia, ship);
	}
	animation_next(&(ship->animation));
	/* Actualizamos su posicion */
	if(ship->limited){
		/* Solo permitimos modificar la posici??n si
			los bordes que definen la nave estan a 15px
			del borde */
		if(!border_into_limits(ship->border,&(ship->limits),
				vector_x(ship->vector),vector_y(ship->vector))){
			return;
		}
	}
	point_add(ship->position,ship->addx,ship->addy);
	weapon_shoot(&(ship->weapon));
}

border_t *ship_border(ship_t *ship){
	return ship->border;
}

uint8_t ship_colision_ship(ship_t *ship, ship_t *ship2){
	if(ship->tangible && ship2->tangible)
		return border_collision(ship->border,ship2->border);
	else
		return false;
}

uint8_t ship_colision_shoot(ship_t *ship, shoot_t *shoot){
	if(ship->tangible)
		return border_collision(ship->border,shoot_get_border(shoot));
	else
		return false;
}

int ship_get_power(ship_t *ship){
	return ship->power;
}

void ship_set_power(ship_t *ship, int power){
	ship->power = power;
}

point_t *ship_get_position(ship_t *ship){
	return ship->position;
}

ia_t *ship_get_ia(ship_t *ship){
	return ship -> ia;
}

void ship_ia_activate(ship_t *ship){
	ship -> ia_activated = 1;
	ia_start(ship -> ia);
}

void ship_destroy(ship_t **ship){
	point_destroy(&((*ship)->position));
	border_destroy(&((*ship)->border));
	vector_destroy(&((*ship)->vector));
	ia_destroy(&((*ship)->ia));
	free(*ship);
}

void ship_set_animation(ship_t *ship, uint8_t sprite,
	uint8_t frame_size, bool loop){
	animation_init(&(ship->animation),sprite,frame_size, loop);
}

void ship_render(ship_t *ship, data_render_t *data, int16_t *sound){
	data->entity_class = ship->type;
	data->pos_x = point_get_x(ship->position);
	data->pos_y = point_get_y(ship->position);
	animation_get(&(ship->animation),&(data->sprite),&(data->frame));

	/* Sound Destroy. All ships */
	if(data->sprite == 1 && data->frame == 9)
		*sound = SOUND_EXPLOSION;		/* Explosion */
	else
		*sound = -1;		/* NADA */
}

/***************************************
				PARA la IA
 ***************************************/

void ia_init(ia_t *ia, clockgame_t *clock){
	ia->path=(lista_t*)malloc(sizeof(lista_t));
	lista_init(ia->path,sizeof(ia_mov_t));
	ia -> clock = clock;
	ia -> next_mov_time = 0;
	ia -> start = false;
}

void ia_start(ia_t *ia){
	lista_first(ia->path);
	ia -> start = true;
}

void ia_add_path( ia_t *ia, uint16_t duration,
					 uint32_t direction, uint32_t speed){
	ia_mov_t *mov;
	mov = (ia_mov_t *)malloc(sizeof(ia_mov_t));
	vector_set(&(mov->vector),vector_grad_to_rad(direction),speed);
	mov->duration = duration;
	lista_add(ia->path,mov);
}

void ia_drive_ship(ia_t *ia, ship_t *ship){
	vector_t *vector;

	if(ia->start){
		if(!lista_eol(ia->path)){
			if(ia->next_mov_time < clockgame_time(ia->clock)){
				vector = &(((ia_mov_t*)(lista_get(ia->path)))->vector);
				ship_set_speed(ship,vector_get_module(vector));
				ship_set_direction(ship,vector_get_direction(vector));
				ia->next_mov_time = ((ia_mov_t*)lista_get(ia->path))->duration + clockgame_time(ia->clock);
				lista_next(ia->path);
			}
		}
	}
}

void ia_mov_destroy(ia_mov_t **ia_mov){
	free(*ia_mov);
}

void ia_destroy(ia_t **ia){
	lista_clean((*ia)->path,(void*)(void**)&ia_mov_destroy);
	free((*ia)->path);
	free(*ia);
}
