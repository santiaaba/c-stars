#include "textures.h"

void textures_init(textures_t *textures){
	textures->sprites = (lista_t*)malloc(sizeof(lista_t));
	lista_init(textures->sprites,sizeof(SDL_Texture));
}

void textures_load_level(textures_t *textures, uint16_t level){
	printf("Implementar\n");
}

SDL_Texture *textures_get(textures_t *textures, uint16_t index){
	return lista_get_by_index(textures->sprites,index);
}

void textures_destroy(textures_t **textures){
	clista_clean((*textures)->sprites,SDL_FreeTexture);
}
