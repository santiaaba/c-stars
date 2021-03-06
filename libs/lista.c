#include "lista.h"

void lista_init(lista_t *l, int e_size){
	l->e_size = e_size;
	l->size = 0;
	l->first = NULL;
	l->last = NULL;
	l->actual = NULL;
}

void lista_add(lista_t *l, void *e){
	/* Agrega un elemento al final de la lista.
	   El puntero actual no se modifica. Excepto que se
	   trate del primer elemento en ingresar a la lista
	   No contempla datos repetidos. Es responsabilidad de
	   la funcion llamadora */

	lista_node *new;
	lista_node *aux;
	new = (lista_node*)malloc(sizeof(lista_node));
	new->next = NULL;
	new->data = e;
	l->size++;
	if(l->first == NULL){
		l->first = new;
		l->last = new;
	} else {
		l->last->next = new;
		l->last = new;
	}
}

void *lista_get(lista_t *l){
	return l->actual->data;
}

void *lista_get_first(lista_t *l){
	return l->first->data;
}

void *lista_get_last(lista_t *l){
	return l->last->data;
}

/* Retorna el elemento en el index indicado.
	No controla si nos caemos de la lista o
	si esta vacia */
void *lista_by_index(lista_t *l,int index){
	int i;
	lista_first(l);
	for(i = 0; i < index; i++){
		lista_next(l);
	}
	return l->actual->data;
}

void lista_first(lista_t *l){
	l->actual = l->first;
}

void lista_next(lista_t *l){
	if(l->actual != NULL){
		l->actual = l->actual->next;
	}
}

unsigned int lista_size(lista_t *l){
	return l->size;
}

unsigned int lista_eol(lista_t *l){
	return (l->actual == NULL);
}

void *lista_exclude(lista_t *l,int (*find_id)(void*), int value){
	/* elimina de la lista el primer elemento donde la funcion
 	   pasada por parametro concida con el tercer valor pasado
	   tambien por parametro. El elemento no se elimina, solo
	   se remueve de la lista. El puntero actual queda apuntando
	   al elemento siguiente */
	   
	int exist = 0;
	lista_node *aux;

	l->actual = l->first;
	while(!exist && l->actual != NULL){
		if((*find_id)(l->actual->data) == value){
			return lista_remove(l);
		} else {
			l->actual = l->actual->next;
		}
	}
	return NULL;
}

void *lista_remove(lista_t *l){
	/* Remueve y retorna el elemento donde apunta
	   el puntero actual elemento  */

	lista_node *prio;
	lista_node *aux;
	void *element = NULL;

	if(l->actual != NULL){
		aux = l->first;
		prio = NULL;
		while(aux != l->actual){
			prio = aux;
			aux = aux->next;
		}
		if(prio == NULL){
			l->first = aux->next;
		} else {
			prio->next = aux->next;
		}
		if(aux == l->last){
			l->last = prio;
		}
		l->actual = aux->next;
		element = aux->data;
		free(aux);
		l->size--;
	}
	return element;
}

void lista_copy(lista_t *l, lista_t *t){
	/* Genera un lista *t con los mismos elementos que en *l.
	 * No se modifica la posicion del puntero actual de *l.
	 * No contempla que lista *t este vacia */

	lista_node *aux;

	aux = l->first;
	while(aux!=NULL){
		lista_add(t,aux->data);
		aux = aux->next;
	}
}

void lista_erase(lista_t *l){
	/* Vacia la lista sin eliminar los elementos */
	lista_first(l);
	while(!lista_eol(l)){
		lista_remove(l);
	}
}

void lista_clean(lista_t *l,void(*clean)(void**)){
	/* Vacia la lista. Los elementos son eliminados.
 	   El segundo parametro es la funcion de borrado */
	lista_node *aux;
	void *element;

	l->actual = l->first;
	while(l->actual != NULL){
		aux = l->actual;
		l->actual = l->actual->next;
		(*clean)(&(aux->data));
		free(aux);
	}
	l->size = 0;
	l->first = NULL;
	l->last = NULL;
	l->actual = NULL;
}

void lista_destroy(lista_t **l,void(*clean)(void**)){
	lista_clean(*l,clean);
	free(*l);
	*l = NULL;
}

void *lista_find(lista_t *l, int (*find_id)(void*), int value){
	/* Retorna el primer elemento que se obtiene de
 	   aplicarle la funci??n pasada por parametro a cada elemento y
	   comparar el resultado con el par??metro value */
	   
	lista_node *aux;
	int exist = 0;

	aux = l->first;
	while(aux != NULL && !exist){
		exist = ((*find_id)(aux->data) ==  value);
		//exist = (proxy_get_id(aux->data) == proxy_id);
		if(!exist){ aux = aux->next;}
	}
	if(exist){
		return aux->data;
	} else {
		return NULL;
	}
}

void *lista_sort(lista_t *l, float (*get_value)(void*), int des){
	/* Ordena la lista. El segundo parametro es la funcion a aplciar a
 	   cada elemento de la lista para obtener un dato ordinal. El tercer
	   parametro indica la direccion del ordenamiento. Si es 0 se ordena
	   de mayor a menor. Si es distinto de 0 se ordena de menor a mayor*/
	   
	void *aux1, *aux2;
	int i,j;

	for(i=0;i<l->size - 1;i++){
		lista_first(l);
		for(j=1;j<l->size - i;j++){
			aux1 = l->actual->data;
			aux2 = l->actual->next->data;
			if(des){
				if((*get_value)(aux1) < (*get_value)(aux2)){
					l->actual->data = aux2;
					l->actual->next->data = aux1;
				}
			} else {
				if((*get_value)(aux1) > (*get_value)(aux2)){
					l->actual->data = aux2;
					l->actual->next->data = aux1;
				}
			}
			lista_next(l);
		}
	}
}
