#include "../libs/lista.h"

void *eliminar(int **i){
	free(*i);
}

void main(){
	lista_t *lista;
	int *n;

	lista = (lista_t*)malloc(sizeof(lista_t));
	lista_init(lista,sizeof(int));
	printf("tamano: %u\n",lista_size(lista));
	n = (int*)malloc(sizeof(int));
	lista_add(lista,n);
	printf("tamano: %u\n",lista_size(lista));
}
