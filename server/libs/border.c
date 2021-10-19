#include "border.h"

/* Inicializa un borde */
void border_init(border_t *border){
	border->lista=(lista_t*)malloc(sizeof(lista_t));
   lista_init(border->lista,sizeof(rect_t));
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

	lista_first(l1);
	while(!lista_eol(l1) && !crash){
		lista_first(l2);
		while(!lista_eol(l2) && !crash){
			crash = rect_collision((rect_t *)lista_get(l1),(rect_t *)lista_get(l2));
		}
	}
	return crash;
}

/* Determina si todos los rectángulos se encuentran dentro de un
	rectángulo mas grande */
bool border_into_limits(border_t *border, rect_t *limit){
	bool into=true;
	printf("border_into_limits() - size: %i\n",lista_size(border->lista));
	lista_first(border->lista);
	while(!lista_eol(border->lista) && into){
		into = rect_into_rect(limit,lista_get(border->lista));
		printf("border_into_limits(x,y,w,h):(%i,%i,%i,%i) into (%i,%i,%i,%i) = %i\n",
				 ((rect_t*)lista_get(border->lista))->point->x,
				 ((rect_t*)lista_get(border->lista))->point->y,
				 ((rect_t*)lista_get(border->lista))->width,
				 ((rect_t*)lista_get(border->lista))->height,
				 limit->point->x,limit->point->y,limit->width,limit->height,into);
		lista_next(border->lista);
	}
	return into;
}

void border_add_vector(border_t* border, vector_t *vector){
	lista_first(border->lista);
	while(!lista_eol(border->lista)){
		point_add_vector(rect_get_point((rect_t *)lista_get(border->lista)),vector);
		lista_next(border->lista);
	}
}

void border_destroy(border_t *border){
	lista_clean(border->lista,(void*)(void**)&rect_destroy);
	free(border->lista);
}
