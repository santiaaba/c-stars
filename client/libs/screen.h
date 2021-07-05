/* Se encarga de renderizar cada frame
	a ser mostrado en el screen */

#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <stdint.h>
#include "../libs/lista.h"
#include "entity.h"

typedef struct t_screen{
	SQL_Surface *play_ground;
	lista_t *entities;
} screen_t;

void screen_init(screen_t *screen);
void screen_window(screen_t *screen, SQL_Window *window);
void screen_draw(screen_t *screen);
lista_t *screen_get_entities(screen_t *screen);
void screen_destroy(screent_t **screen);

#endif
