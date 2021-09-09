/* Input de texto */

#include "input.h"

void input_init(input_t *input,
							int x,
							int y,
							int focus,
							SDL_Renderer *renderer){

	input->off = IMG_LoadTexture(renderer, "img/input_off.png");
	input->on = IMG_LoadTexture(renderer, "img/input_on.png");
	input->alphabet = IMG_LoadTexture(renderer, "img/alfabeto.png");

	input->focus = focus;

	input->frame.x = 0;
	input->frame.y = 0;
	input->frame.w = TEXT_WIDTH;  /* Ancho fijo del input */
	input->frame.h = TEXT_HEIGHT;		/* Alto fijo al input */

	input->dest.x = x;
	input->dest.y = y;
	input->dest.w = TEXT_WIDTH;		/* Ancho fijo al input */
	input->dest.h = TEXT_HEIGHT;	 	/* Alto fijo al input */

	strcpy(input->text,"");

	input->renderer = renderer;
}

void input_draw(input_t *input){
	int i;
	SDL_Rect char_frame;
	SDL_Rect char_dest;

	if(input->focus){
		SDL_RenderCopy(input->renderer,input->on,&(input->frame),&(input->dest));
	} else {
		SDL_RenderCopy(input->renderer,input->off,&(input->frame),&(input->dest));
	}
	/* Ahora dibujamos el texto */
	char_frame.y = 0;
	char_frame.w = CHAR_WIDTH;
	char_frame.h = CHAR_HEIGHT;

	char_dest.x = input->dest.x + 5;		//padding 5
	char_dest.y = input->dest.y + 5;		// padding 10
	char_dest.w = CHAR_WIDTH;
	char_dest.h = CHAR_HEIGHT;
	for(i=0;i<strlen(input->text);i++){
		switch(input->text[i]){
			case '0':
				char_frame.x = 0;
				break;
			case '1':
				char_frame.x = CHAR_WIDTH;
				break;
			case '2':
				char_frame.x = 2 * CHAR_WIDTH;
				break;
			case '3':
				char_frame.x = 3 * CHAR_WIDTH;
				break;
			case '4':
				char_frame.x = 4 * CHAR_WIDTH;
				break;
			case '5':
				char_frame.x = 5 * CHAR_WIDTH;
				break;
			case '6':
				char_frame.x = 6 * CHAR_WIDTH;
				break;
			case '7':
				char_frame.x = 7 * CHAR_WIDTH;
				break;
			case '8':
				char_frame.x = 8 * CHAR_WIDTH;
				break;
			case '9':
				char_frame.x = 9 * CHAR_WIDTH;
				break;
			case '.':
				char_frame.x = 10 * CHAR_WIDTH;
				break;
		}
		SDL_RenderCopy(input->renderer,input->alphabet,&char_frame,&char_dest);
		char_dest.x += CHAR_WIDTH;
	}
}

void input_add_char(input_t *input, char c){
	int i = strlen(input->text);
	printf("Agregando %c\n",c);
	if(i < TEXT_LIMIT){
		input->text[i] = c;
		input->text[i+1] = '\0';
	}
	printf("Agregando:%s\n",input->text);
}

void input_del_char(input_t *input){
	int i = strlen(input->text);
	if(i > 0)
		input->text[i-1] = '\0';
	printf("Borrado:%s\n",input->text);
}

char *input_get_value(input_t *input){
	return input->text;
}
