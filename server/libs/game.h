#ifndef GAME_H
#define GAME_H

#define BUFFER_SIZE 200			/* elementos a renderizar */
#define EVENT_LIMIT_SIZE  10	/* Limite de eventos */
#define NANOTIME 60000

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
#include "level.h"
#include "game.h"
#include "clockgame.h"

#define	G_WAIT_CONNECT				0
#define	G_CONNECT_STEP_ONE		1
#define	G_READY						2
#define	G_PLAYING					3
#define	G_PAUSE						4
#define	G_STOP						5
#define	G_LEAVE						6

#define K_TOP					0
#define K_BOTTOM				1
#define K_LEFT					2	
#define K_RIGHT				3
#define K_SPACEBAR			4

#define K_DOWN			0
#define K_UP			1

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
	int event_size;						/* Tamano logico del vector key */
	int status;
	uint32_t score;
	sem_t *sem_event;						/* Semaphore para la zona critica de eventos */
	render_t buffer[BUFFER_SIZE];			/* Buffer para el envio UDP */
	int buffer_size; 	 	 				/* Tamano logico del buffer para UDP */
	key_direction_t direction;
	ship_t *player;
	lista_t *enemies;
	lista_t *shoot_enemies;
	lista_t *shoot_player;
	level_t *level;
	clockgame_t *clock;
} game_t;

void game_init(game_t *g, sem_t *sem_event);
void game_event_add(game_t *g, game_event_t *e);
void game_run(game_t *g);
void game_set_level(game_t *g, int level);
void game_start(game_t *g);
void game_pause(game_t *g);
void game_udp_negociate(game_t *g);
void game_over(game_t *g);
int game_get_state(game_t *g);
void game_set_state(game_t *g, int state);

#endif
