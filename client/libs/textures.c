#include "textures.h"

void textures_init(textures_t *textures){
}

void textures_load_level(textures_t *textures, uint16_t level){
	printf("Implementar\n");
}

SDL_Texture *textures_get(textures_t *textures, uint16_t index){
	return textures->sprites[index];
}

void textures_destroy(textures_t **textures){
	uint16_t i;
	for(i=0;i<(*textures)->size;i++){
		SDL_DestroyTexture((*textures)->sprites[i]);
	}
	free(*textures);
}
