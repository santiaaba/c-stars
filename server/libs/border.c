#include "border.h"

/* Inicializa un borde */
void border_init(border_t *border, int xx, int yy){
	border->lista=(lista_t*)malloc(sizeof(lista_t));
   lista_init(border->lista,sizeof(rect_t));
	border->xx = xx;
	border->yy = yy;
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

	lista_first(l1);
	while(!lista_eol(l1) && !crash){
		rect_copy(&rect1,(rect_t *)lista_get(l1));
		rect_get_point(&rect1,&x,&y);
		x += border->xx;
		y += border->yy;
		rect_set_point(&rect1,x,y);

		lista_first(l2);
		while(!lista_eol(l2) && !crash){
			rect_copy(&rect2,(rect_t *)lista_get(l2));
			rect_get_point(&rect2,&x,&y);
			x += border2->xx;
			y += border2->yy;
			rect_set_point(&rect2,x,y);
			crash = rect_collision(&rect1,&rect2);
			lista_next(l2);
		}
		lista_next(l1);
	}
	return crash;
}

void border_set_point(border_t *border, int32_t xx, int32_t yy){
	border->xx = xx;
	border->yy = yy;
}

/* Determina si todos los rectángulos se encuentran dentro de un
	rectángulo mas grande */
bool border_into_limits(border_t *border, rect_t *limit, int32_t add_x, int32_t add_y){
	bool into=true;
	uint32_t x,y;
	rect_t rect;
//	printf("border_into_limits() - size: %i\n",lista_size(border->lista));
	lista_first(border->lista);
	while(!lista_eol(border->lista) && into){
		rect_copy(&rect,(rect_t*)lista_get(border->lista));
		rect_get_point(&rect,&x,&y);
		rect_set_point(&rect,x + border->xx + add_x,y + border->yy + add_y);
		into = rect_into_rect(limit,&rect);
//		printf("border_into_limits(x,y,w,h):(%i,%i,%i,%i) into (%i,%i,%i,%i) = %i\n",
//				 rect.x, rect.y, rect.width, rect.height,
//				 limit->x,limit->y,limit->width,limit->height,into);
		lista_next(border->lista);
	}
	return into;
}

void border_add_vector(border_t* border, vector_t *vector){
	border->xx += vector_x(vector);
	border->yy += vector_y(vector);
}

void border_destroy(border_t **border){
	lista_clean((*border)->lista,(void*)(void**)&rect_destroy);
	free((*border)->lista);
	free(*border);
}
