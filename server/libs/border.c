#include "border.h"

/* Inicializa un borde */
void border_init(border_t *border, point_t *position, int w, int h){
	border->lista=(lista_t*)malloc(sizeof(lista_t));
   lista_init(border->lista,sizeof(rect_t));
	border->position = position;
	rect_init(&(border->big));
	rect_set_point(&(border->big),0,0);
	rect_set_dim(&(border->big),w,h);
}

/* Agrega un rectangulo a la lista */
void border_add_rect(border_t *border, rect_t *rect){
	lista_add(border->lista,rect);
}

/* Determina si ha habido colicion entre los dos bordes */
uint8_t border_collision(border_t *border, border_t *border2){
	lista_t *l1 = border->lista;
	lista_t *l2 = border2->lista;
	uint8_t crash = 0;
	/* Los rec_t de los bordes tiene coordenadas
		relativas. Para no modificarlos, generamos
		un par de auxliliarles a los que les sumamos
		las coordenadas xx e yy respectivas. */
	rect_t rect1;
	rect_t rect2;
	int32_t x;
	int32_t y;

	/* Primero determinamos si los big de cada borde
		se tocan. Si se tocan... entonces analizamos
		los de adentro */

	rect_copy(&rect1,&(border->big));
	x = point_get_x(border->position);
	y = point_get_y(border->position);
	rect_set_point(&rect1,x,y);

	rect_copy(&rect2,&(border2->big));
	x = point_get_x(border2->position);
	y = point_get_y(border2->position);
	rect_set_point(&rect2,x,y);

	//printf("Comparando bigs:(x:%i,y:%i,w:%i,h:%i),(x:%i,y:%i,w:%i,h:%i)\n",
//	rect1.x,rect1.y,rect1.width,rect1.height,rect2.x,rect2.y,rect2.width,rect2.height);
	if(!rect_collision(&rect1,&rect2)){
//		printf("BIGS no colicionan!!\n");
		return false;
	}
/*	printf("border_collision(): %i,%i\n",
			point_get_x(border->position),
			point_get_y(border->position));
*/
	lista_first(l1);
	while(!lista_eol(l1) && !crash){
		rect_copy(&rect1,(rect_t *)lista_get(l1));
		rect_get_point(&rect1,&x,&y);
		x += point_get_x(border->position);
		y += point_get_y(border->position);
		rect_set_point(&rect1,x,y);
		lista_first(l2);
		while(!lista_eol(l2) && !crash){
			rect_copy(&rect2,(rect_t *)lista_get(l2));
			rect_get_point(&rect2,&x,&y);
			x += point_get_x(border2->position);
			y += point_get_y(border2->position);
			rect_set_point(&rect2,x,y);
			crash = rect_collision(&rect1,&rect2);
			lista_next(l2);
		}
		lista_next(l1);
	}
	return crash;
}

bool border_into_limits(border_t *border, rect_t *limit, int32_t add_x, int32_t add_y){
/* Determina si todos los rectángulos que conforman el borde se encuentran
	dentro de un rectángulo mas grande */
	bool into=true;
	int x,y;
	rect_t rect;
//	printf("border_into_limits() - size: %i\n",lista_size(border->lista));
	lista_first(border->lista);
	while(!lista_eol(border->lista) && into){
		rect_copy(&rect,(rect_t*)lista_get(border->lista));
		rect_get_point(&rect,&x,&y);
		rect_set_point(&rect,
							x + point_get_x(border->position) + add_x,
							y + point_get_y(border->position) + add_y);
		into = rect_into_rect(limit,&rect);
/*		printf("border_into_limits(x,y,w,h):(%i,%i,%i,%i) into (%i,%i,%i,%i) = %i\n",
				 rect.x, rect.y, rect.width, rect.height,
				 limit->x,limit->y,limit->width,limit->height,into);*/
		lista_next(border->lista);
	}
	return into;
}

bool border_out_limits(border_t *border, rect_t *limit){
	/* Determina si el borde big se encuentra por fuera de los limites */
	rect_t rect;
	int x,y;

	rect_copy(&rect,&(border->big));
	rect_get_point(&rect,&x,&y);
	rect_set_point(&rect,x + point_get_x(border->position),
		y + point_get_y(border->position));

	printf ("---------out?----------\n");
	return rect_out_rect(&rect,limit);
	
}

void border_destroy(border_t **border){
	lista_clean((*border)->lista,(void*)(void**)&rect_destroy);
	free((*border)->lista);
	free(*border);
}
