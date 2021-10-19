#include "ship.h"

void ship_init(ship_t *ship, uint8_t type, clockgame_t *clock){
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

	ship -> power = 0;
	ship -> speed = 1;

	ship->position=(point_t*)malloc(sizeof(point_t));
	ship->border=(border_t*)malloc(sizeof(border_t));
	ship->vector=(vector_t*)malloc(sizeof(vector_t));
	ship->ia=(ia_t*)malloc(sizeof(ia_t));

	point_init(ship->position);
	border_init(ship->border);
	vector_init(ship->vector);
	ia_init(ship->ia, clock);

	ship->type = type;
	ship->ia_activated = 0;
	ship->state = SHIP_LIVE;

	/* Asignamos los bordes y las animaciones de inicio */
	switch(type){
		case PLAYER:
			rect = create_rect(21,4,21,80);
			border_add_rect(ship->border,rect);
			rect = create_rect(42,17,17,53);
			border_add_rect(ship->border,rect);
			rect = create_rect(4,33,17,23);
			border_add_rect(ship->border,rect);
			animation_init(&(ship->animation),0,1,false);
			ship->limited = true;
			break;
		case ENEMIE1:
			rect = create_rect(20,40,30,61);
			border_add_rect(ship->border,rect);
			rect = create_rect(50,10,56,120);
			border_add_rect(ship->border,rect);
			animation_init(&(ship->animation),0,1,false);
			ship->limited = false;
			break;
		case ENEMIE2:
			rect = create_rect(8,19,126,23);
			border_add_rect(ship->border,rect);
			rect = create_rect(19,90,126,23);
			border_add_rect(ship->border,rect);
			rect = create_rect(30,37,114,47);
			border_add_rect(ship->border,rect);
			animation_init(&(ship->animation),0,1,false);
			ship->limited = false;
			break;
	}
}

void ship_set_limits(ship_t *ship,int limit_bottom, int limit_right){
	rect_set_dim(&(ship->limits),limit_right,limit_bottom);
}

void ship_set_state(ship_t *ship, uint8_t state){
	ship->state = state;
}

uint8_t ship_get_state(ship_t *ship){
	return ship->state;
}

void ship_set_speed(ship_t *ship, float speed){
	ship->speed = speed;
}

float ship_get_speed(ship_t *ship){
	return ship->speed;
}

void ship_set_position(ship_t *ship, int32_t x, int32_t y){
	point_set( ship->position,x,y);
}

void ship_move(ship_t *ship){
	/* Modificamos el vector de movimiento
		si la ia lo ordenase */
	printf("ship_move() - (%i,%i)\n",
		ship->position->x,ship->position->y);
	if(ship -> ia_activated){
		ia_drive_ship(ship->ia, ship);
	}
	animation_next(&(ship->animation));
	/* Actualizamos su posicion */
	if(ship->limited){
			printf("ship_move(): limited\n");
		/* Solo permitimos modificar la posición si
			los bordes que definen la nave estan a 15px
			del borde */
		if(!border_into_limits(ship->border,&(ship->limits))){
			printf("FUERA DE LOS LIMITES\n");
			return;
		}
	}

	printf("ship_move() ----> (M:%f,D:%i)=(%i,%i) ----> ",
		ship->vector->modulo,ship->vector->direccion,
		vector_x(ship->vector),vector_y(ship->vector));
	point_add_vector(ship->position,ship->vector);
	printf("(%i,%i)\n",ship->position->x,ship->position->y);
	border_add_vector(ship->border,ship->vector);
}

void ship_set_vector(ship_t *ship, vector_t *vector){
	vector_copy(ship->vector,vector);
}

vector_t *ship_get_vector(ship_t *ship){
	return ship->vector;
}

border_t *ship_border(ship_t *ship){
	return ship->border;
}

uint8_t ship_colision_ship(ship_t *ship, ship_t *ship2){
	return border_collision(ship->border,ship_border(ship2));
}

uint8_t ship_colision_shoot(ship_t *ship, shoot_t *shoot){
	return border_collision(ship->border,shoot_get_border(shoot));
}

void ship_border_add(ship_t *ship, int32_t x, int32_t y,
							uint32_t width, uint32_t height){
	rect_t *rect;
	rect = (rect_t *)malloc(sizeof(rect_t));
	rect_init(rect);
	rect_set_point(rect,x,y);
	rect_set_dim(rect,width,height);
	border_add_rect(ship->border,rect);
}

uint32_t ship_get_power(ship_t *ship){
}

void ship_set_power(ship_t *ship, uint32_t power){
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

void ship_destroy(ship_t *ship){
	point_destroy(ship->position);
	border_destroy(ship->border);
	vector_destroy(ship->vector);
	ia_destroy(ship->ia);

	free(ship->position);
	free(ship->border);
	free(ship->vector);
	free(ship->ia);
}

void ship_set_animation(ship_t *ship, uint8_t sprite,
	uint8_t frame_size, bool loop){
	animation_init(&(ship->animation),sprite,frame_size, loop);
}

void ship_data(ship_t *ship, data_render_t *data){
	data->entity_class = ship->type;
	data->pos_x = point_get_x(ship->position);
	data->pos_y = point_get_y(ship->position);
	animation_get(&(ship->animation),&(data->sprite),&(data->frame));
}

void ship_render(ship_t *ship, data_render_t *data){
	data->entity_class = ship->type;
	data->pos_x = point_get_x(ship->position);
	data->pos_y = point_get_y(ship->position);
	animation_get(&(ship->animation),&(data->sprite),&(data->frame));
}

/***************************************
				PARA la IA
 ***************************************/

void ia_init(ia_t *ia, clockgame_t *clock){
	ia->path=(lista_t*)malloc(sizeof(lista_t));
	lista_init(ia->path,sizeof(ia_mov_t));
	ia -> clock = clock;
}

void ia_start(ia_t *ia){
	printf("ia_start()\n");
	lista_first(ia->path);
	ia -> time_start = clockgame_time(ia->clock);
}

void ia_add_path( ia_t *ia, uint16_t instant,
					 uint32_t direction, uint32_t speed){
	printf("ia_add_path(): Cargando vector\n");
	ia_mov_t *mov;
	mov = (ia_mov_t *)malloc(sizeof(ia_mov_t));
	vector_set(&(mov->vector),direction,speed);
	mov->instant = instant;
	printf("ia_add_path(): Agregando vector a la lista\n");
	lista_add(ia->path,mov);
	printf("ia_add_path(): Vector agregado\n");
}

void ia_drive_ship(ia_t *ia, ship_t *ship){
	if(!lista_eol(ia->path)){
		printf("ia_drive_ship(): Existen aun rutas\n");
		if(((ia_mov_t*)lista_get(ia->path))->instant + ia->time_start
			<= clockgame_time(ia->clock)){
			printf("ia_drive_ship(): Ruta aplicada\n");
			ship_set_vector(ship,&(((ia_mov_t*)(lista_get(ia->path)))->vector));
			lista_next(ia->path);
		}
	} else {
		printf("ia_drive_ship(): NO hay mas rutas. Eliminamos nave\n");
		/* Si no hay mas elementos en la lista entonces seteamos
			el estado de la nave para que sea destruida. Ésto
			implica que todo path de las naves debe tener al
			menos una última instrucción inservible */
		ship_set_state(ship,SHIP_DESTROY);
	}
}

void ia_mov_destroy(void **ia_mov){
	free((ia_mov_t*)(*ia_mov));
}

void ia_destroy(ia_t *ia){
	/* OJO... no eliminar el clock. Ya que es utilizado
	   por otras estructuras */
	lista_clean(ia->path,(void*)(void**)&ia_mov_destroy);
}
