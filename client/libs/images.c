#include "images.h"

void images_init(images_t *images){
	images->sprites = (lista_t*)malloc(sizeof(lista_t));
	lista_init(images->sprites,sizeof(SDL_Surface));
}

void images_add_sprite(images_t *images, SDL_Surface *sprite){
	lista_add(images->sprites,sprite);
}

SDL_Sprite *images_get_sprite(images_t *images, uint16_t index){
	return lista_get_by_index(images->sprites,index);
}

void images_destroy(images_t **images){
	clista_clean((*images)->sprites,SDL_FreeSurface);
}
