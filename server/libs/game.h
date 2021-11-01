#ifndef GAME_H
#define GAME_H

#define BUFFER_SIZE			200			/* elementos a renderizar */
#define MAX_UDP_BUFFER		200			/* Tamano maximo de buffer UDP */
#define CANT_LEVELS			1
#define FXS						2				/*Fotogramas por segundo */
#define EVENT_LIMIT_SIZE	10
#define SCREEN_WIDTH			1024
#define SCREEN_HEIGHT		600

//Coordenadas en radianes
#define G_0		0
#define G_45	0.785398
#define G_90	1.5708
#define G_135	2.26893
#define G_180	3.14159
#define G_225	3.92699
#define G_270	4.71239
#define G_315	5.49779

/* Cada dato de render a enviar al cliente tiene el siguiente formato 

     uint16_t	int16_t		int16_t		uint8_t		uint8_t
  -----------------------------------------------------------
 |entity-type | pos-x		 | pos-y		 | sprite	|	 frame |
  -----------------------------------------------------------

Son en total 8 bytes
*/
#define DATA_RENDER_SIZE 8

#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include "point.h"
#include "ship.h"
#include "shoot.h"
#include "vector.h"
#include "level.h"
#include "game.h"
#include "clockgame.h"
#include "../../libs/game_state.h"
#include "../../libs/eaeapp.h"
#include "../../libs/types.h"
#include "../../libs/game_info.h"

#define G_MAX_ENTITY		100 //Cantidad maxima de entidades permitidas

#define K_TOP					82
#define K_BOTTOM				81
#define K_LEFT					80
#define K_RIGHT				79
#define K_SPACEBAR			4

#define K_DOWN			768
#define K_UP			769

#define PLAYER_MODULE	10.0

typedef struct {
	bool top;
	bool bottom;
	bool left;
	bool right;
} key_direction_t;

typedef struct {
	int key;
	int key_type;
} game_event_t;

typedef struct {
	game_event_t *events[EVENT_LIMIT_SIZE];
	int event_size;					/* Tamano logico del vector key */
	int state;
	uint32_t frame;
	uint32_t score;
	bool statusForced;
	sem_t *sem_event;					/* Semaphore para la zona critica de eventos */
	sem_t sem_state;					/* Semaphore para la zona critica de state */
	data_t data;						/* Datos a enviar por udp */
	char *buffer; 		 	 			/* Buffer udp */
	int buffer_size;
	key_direction_t direction;
	ship_t *player;
	bool request_status;				/* 1 si requiere que el cliente solicite el estado */
	lista_t *enemies;
	lista_t *shoot_enemies;
	lista_t *shoot_player;
	level_t *level;
	int level_current;				/* Nivel actual */
	clockgame_t *clock;
	int sockfd;
	struct sockaddr_in servaddr;
} game_t;

void game_init(game_t *g, sem_t *sem_event);
void game_event_add(game_t *g, uint16_t key, uint16_t key_type);
void game_start(game_t *g);
int game_init_udp(game_t *g, char *ip, int port);
void *game_run(void *g);
void game_level_start(game_t *g, int level);
void game_info(game_t *g, game_info_t *info);
void game_pause(game_t *g);
void game_resume(game_t *g);
void game_stop(game_t *g);
int game_udp_negociate(game_t *g, char *ip, int port);
void game_over(game_t *g);
int game_get_state(game_t *g);
void game_set_state(game_t *g, int state);

#endif
