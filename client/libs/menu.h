/* Menu */

#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <string.h>
#include "button.h"

#define MAX_OPTIONS  6
#define FONT "fonts/turret-road.extrabold.ttf"
#define M_BUTTON_W 400
#define M_BUTTON_H 50

/* Colores de background boton sin foco */
#define B_BG_R	100
#define B_BG_G	10
#define B_BG_B	255

/* Colores de background boton con foco */
#define B_BG_F_R	100
#define B_BG_F_G	100
#define B_BG_F_B	100

/* Colores de fuente boton sin foco */
#define B_TEXT_R	0
#define B_TEXT_G	0
#define B_TEXT_B	0

/* Colores de fuente boton con foco */
#define B_TEXT_F_R	200
#define B_TEXT_F_G	200
#define B_TEXT_F_B	0

/* Colores de borde boton sin foco */
#define B_BORDE_R	0
#define B_BORDE_G	0
#define B_BORDE_B	0

/* Colores de borde boton con foco */
#define B_BORDE_F_R	200
#define B_BORDE_F_G	200
#define B_BORDE_F_B	0


typedef struct {
	SDL_Rect dest;
	int show;
	int looked;
	button_t *options[MAX_OPTIONS];
	int size;
	int index;
	SDL_Renderer *renderer;
} menu_t;

void menu_init(menu_t *menu, int x, int y, SDL_Renderer *renderer);
void menu_down(menu_t *menu);
void menu_up(menu_t *menu);
void menu_draw(menu_t *menu);
void menu_add(menu_t *menu, char *text);
void menu_show(menu_t *menu);
void menu_hidden(menu_t *menu);
int menu_get_index(menu_t *menu);
int menu_is_looked(menu_t *menu);
void menu_unlooked(menu_t *menu);
void menu_looked(menu_t *menu);
void menu_destroy(menu_t *menu);

#endif
