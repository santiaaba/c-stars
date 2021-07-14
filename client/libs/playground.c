#include "playground.h"

int playground_init(playground_t *playground){
	playground->entities = 0;
	playground->last_frame = 0;
}

void playground_draw(playground_t *playground, int clear, SDL_Renderer *renderer){
	/* Dibuja entidades segun lo almacenado en el buffer.
		Si el segundo parametro es un true, borra el buffer luego
		de dibujar la pantalla. Sino lo mantiene. */
	uint16_t i;
	for(i=0; i < (playground->entities); i++)
		entity_draw(&(playground->buffer[i]),renderer);
	if(clear)
		playground->entities = 0;
}

void playground_clear_entities(playground_t *playground){
	playground->entities = 0;
}

void playground_add_entity(playground_t *playground, entity_t *entity){
	/* Agrega entidades segun los datos recibidos por UDP */
	if(playground->entities < ENTITIES_LIMIT){
		entity_copy(&(playground->buffer[playground->entities]),entity);
		playground->entities ++;
	} else {
		printf("Playgroud: No caben mas entidades en el array de entidades\n");
	}
}
