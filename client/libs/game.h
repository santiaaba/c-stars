#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include "entity.h"
#include "button.h"
#include "powerbar.h"
#include "background.h"
#include "input.h"
#include "menu.h"
#include "text.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "../../libs/eaeapp.h"
#include "../../libs/types.h"
#include "../../libs/game_state.h"
#include "../../libs/level_state.h"
#include "./tcp_client.h"

#define SRV_PORT			2525
#define SCREEN_WIDTH		1024
#define SCREEN_HEIGHT	600
#define SCREEN_BPP		24
#define SCREEN_REFRESH	60
#define CANT_TEXTURES	3
#define CANT_SOUNDS		2
#define CANT_FX_SOUNDS	2
#define CANT_LEVELS		2

/* Estados del cliente */
#define HELLO				0		// Pantalla de presentacion
#define DISCONNECTED		1		// Pantalla de conexion
#define CONNECTED			2		// Conectado listo para iniciar
#define PLAYING			3		// Pantalla de partida donde se juega
#define PAUSE				4		// Pausado
#define END					5		// Finaliza el cliente
#define END_LEVEL			6		// Finaliza el nivel
#define END_GAME			7		// Finaliza el juego

/* Efectos de sonido */
#define FX_MENU			0
#define FX_CLICK			1

typedef struct{
	SDL_Texture *texture;
	int w;	/* ancho del tile */
	int h;	/* alto del tile */
} entitie_t;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	entitie_t entities[CANT_TEXTURES];			// Entidades
	SDL_Texture *background_temp;
	SDL_Texture *backgrounds[CANT_LEVELS];		// Background
	Mix_Chunk *sounds[CANT_SOUNDS];				// Sonidos provenientes del servidor
	Mix_Chunk *fx_sounds[CANT_FX_SOUNDS];		// Sonidos varios
	uint16_t screen_frame;							// Frame actual
	int status;											// Estado del cliente
	bool status_in_progress;

	/* Los siguientes 4 atributos son obtenidos
		mediante el mensaje game_status al servidor */

	uint8_t game_state;			// Estado del juego segun el server
	uint8_t power_ship;				// energia de la nave
	uint8_t level;						// Nivel actual
	uint8_t level_state;				// Estado del nivel
	uint32_t score;					// Puntaje

	data_t data;
	tcp_client_t *command_cli;
	sem_t sem_status;
	pthread_t th_status;
	int udp;
	int sockfd;
	struct sockaddr_in cliaddr;
	struct sockaddr_in servaddr;
	data_t *buffer_render;		 // Buffer render
	int buffer_render_size;
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
