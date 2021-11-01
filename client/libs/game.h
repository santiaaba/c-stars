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

/* EStados del cliente */
#define HELLO				0		// Pantalla de presentacion
#define DISCONNECTED		1		// Pantalla de conexion
#define CONNECTED			2		// Conectado listo para iniciar
#define PLAYING			3		// Pantalla de partida donde se juega
#define PAUSE				4		// Pausado
#define END					5		// Finaliza el cliente
#define END_LEVEL			6		// Finaliza el nivel
#define END_GAME			7		// Finaliza el juego

typedef struct{
	SDL_Texture *texture;
	int w;	/* ancho del tile */
	int h;	/* alto del tile */
} entitie_t;

typedef struct {
	SDL_Window *window;
	SDL_Renderer *renderer;
	entitie_t entities[CANT_TEXTURES];		// Texturas
	uint16_t screen_frame;						// Frame actual
	int status;							// Estado del cliente
	bool status_in_progress;

	/* Los siguientes 4 atributos son obtenidos
		mediante el mensaje game_status al servidor */

	uint16_t playing_state;			// Estado segun el server
	uint8_t level;						// Nivel actual
	uint8_t level_state;			// Estado del nivel
	uint32_t score;					// Puntaje

	tcp_client_t *command_cli;
	sem_t sem_status;
	pthread_t th_render;
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
