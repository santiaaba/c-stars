#include "border.h"

/* Inicializa un borde */
void border_init(border_t *border){
   border->lista = (lista_t*)malloc(sizeof(lista_t));
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

void border_add_vector(border_t* border, vector_t *vector){
	lista_first(border->lista);
	while(!lista_eol(border->lista)){
		point_add_vector(rect_get_point((rect_t *)lista_get(border->lista)),vector);
		lista_next(border->lista);
	}
}

void border_destroy(void **border){
	lista_clean(((border_t *)(*border))->lista,&rect_destroy);
	free(*border);
}
