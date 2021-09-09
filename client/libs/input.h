/* Input de texto */

#ifndef INPUT_H
#define INPUT_H

#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <string.h>

#define TEXT_LIMIT 20
#define TEXT_WIDTH 428
#define TEXT_HEIGHT 60
#define CHAR_WIDTH 33
#define CHAR_HEIGHT 40

typedef struct {
	SDL_Rect dest;
	SDL_Rect frame;
	SDL_Renderer *renderer;
	SDL_Texture *off;
	SDL_Texture *on;
	int focus;
	SDL_Texture *alphabet;
	char text[TEXT_LIMIT];			// Solo numerico y "." para poder generar una IP y limite 20
} input_t;

void input_init(input_t *input, int x, int y, int focus, SDL_Renderer *renderer);

void input_draw(input_t *input);

void input_add_char(input_t *input, char c);

void input_del_char(input_t *input);

char *input_get_value(input_t *input);

#endif
