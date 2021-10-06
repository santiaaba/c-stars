/* Input de texto */

#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define FONT "fonts/turret-road.extrabold.ttf"
#define BUTTON_FONT_SIZE 20

typedef struct {
	SDL_Rect dest;
	SDL_Renderer *renderer;
	TTF_Font *font;
	char *text;
	SDL_Color font_color;
	SDL_Color font_color_focus;
	SDL_Color bg_color;
	SDL_Color bg_color_focus;
	SDL_Color border_color;
	SDL_Color border_color_focus;
	SDL_Texture *texture;
	int focus;
} button_t;

void button_init(button_t *button, int x, int y,
						int w, int h, int focus,
						SDL_Renderer *renderer);

void button_text(button_t *button, char *text);

void button_bg_color(button_t *button,
							uint8_t r, uint8_t g, uint8_t b,
							uint8_t focus_r, uint8_t focus_g,
							uint8_t focus_b);

void button_font_color(button_t *button,
							  uint8_t r, uint8_t g, uint8_t b,
							  uint8_t focus_r, uint8_t focus_g,
							  uint8_t focus_b);

void button_border_color(button_t *button,
								 uint8_t r, uint8_t g, uint8_t b,
								 uint8_t focus_r,uint8_t focus_g,
								 uint8_t focus_b);

void button_draw(button_t *button);

void button_focus(button_t *button);

void button_exit(button_t *button);

#endif
