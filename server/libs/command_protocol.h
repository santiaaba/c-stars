#ifndef COMMAND_PROTOCOL_H
#define COMMAND_PROTOCOL_H

#include "./game.h"

/* Version minima aceptada */
#define VERSION			1

/* Los diferentes mensajes que puede recibir el servidor */
#define C_CONNECT_1		0
#define C_CONNECT_2		1
#define C_DISCONNECT		2
#define C_KEEPALIVE		3
#define C_KEY_PRESS		4
#define C_GAME_START		5
#define C_GAME_STOP		6
#define C_GAME_PAUSE		7
#define C_GAME_RESUME	8
#define C_GAME_STATUS	9

#define RES_OK					0
#define RES_INCORRECT		1
#define RES_ERROR_VERSION	2

/* Estado del protocolo del lado del servidor */

typedef struct req_connect {
	uint16_t udp;
	uint16_t version;
} req_connect_t;

typedef struct req_key_press {
	uint16_t key;		// Codigo ASSCI tecla presionada
	uint16_t mode;		// Presionada o soltada
} req_key_press_t;

typedef uint32_t req_level_t ;

typedef struct {
	uint8_t cod;
	uint8_t aux;
	uint16_t size;
	uint32_t qid;
} req_header_t;

typedef struct {
	req_header_t header;
	void *body;			// Los body varian en base al mensaje
} req_t;

/* Para las respuestas */
typedef struct {
	uint8_t cod;
	uint8_t resp;
	uint16_t size;
	uint32_t qid;
} res_header_t;

typedef struct {
	res_header_t header;
	void *body;		// Los body varian en base al mensaje
} res_t;

//void server_protocol_init(server_protol_t *p, game_t *game);
void server_protocol_handle(game_t *g, char *req_buffer,
									 int req_size, char *res_buffer,
									 int *res_size);
#endif
