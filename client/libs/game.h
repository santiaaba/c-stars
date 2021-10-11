#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "entity.h"
#include "button.h"
#include "input.h"
#include "text.h"
#include "../../libs/eaeapp.h"
#include "./tcp_client.h"

#define SRV_PORT			2525
#define SCREEN_WIDTH    1024
#define SCREEN_HEIGHT   600
#define SCREEN_BPP      24
#define SCREEN_REFRESH	60

/* EStados del cliente */
#define HELLO			0		// Pantalla de presentacion
#define CONNECT		1		// Pantalla de conexion
#define MAINMENU		2		// Pantalla de menu una vez conectado
#define PLAYING		3		// Pantalla de partida
#define PAUSE			4		// Pantalla de pausa
#define END				5		// Finaliza el cliente

typedef struct{
	SDL_Texture *texture;
	int w;
	int h;
} entities_t;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	uint16_t screen_frame;				// En estado PLAYING, determina el frame actual
	int status;
	tcp_client_t *command_cli;
	entities_t entities[10];
	sem_t *sem_render;
	int udp;
	sem_t sem_status;
	data_t *buffer_render;			// Buffer render
	int buffer_render_size;			// Cantidad de elementos en el buffer render
} game_t;

int game_init(game_t *game);
void *game_run(game_t *game);
void game_hello(game_t *game);
void game_connect(game_t *game);
void game_main_menu(game_t *game);
void game_play(game_t *game);
void game_pause(game_t *game);
void game_free(game_t *game);

#endif
