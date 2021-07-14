#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "entity.h"
#include "button.h"
#include "input.h"

#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   600
#define SCREEN_BPP      24

typedef enum {
	HELLO,
	CONNECT,
	MAINMENU,
	PLAY,
	PAUSE,
	END
} game_status_t;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	game_status_t status;
} game_t;

int game_init(game_t *game);
void game_run(game_t *game);
void game_hello(game_t *game);
void game_connect(game_t *game);
void game_main_menu(game_t *game);
void game_play(game_t *game);
void game_pause(game_t *game);
void game_free(game_t *game);

#endif
