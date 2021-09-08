#ifndef EAEAPP_H
#define EAEAPP_H

#include <stdint.h>

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
#define MAX_DATA_BODY		200

/*********************************************/
/* Estructuas para el envio de datos por UDP */
/*********************************************/
typedef struct {
	uint32_t frame;
	uint8_t type;
	uint16_t size;
	uint8_t aux;
} data_header_t;

typedef struct {
	uint16_t entity_class;
	int16_t pos_x;
	int16_t pos_y;
	uint8_t sprite;
	uint8_t frame;
} data_body_t;

typedef struct {
	data_header_t header;
	data_body_t body[MAX_DATA_BODY];
} data_t;

/************************************************/
/* Estructuas para el envio de comandos por TCP */
/************************************************/
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

/* Para el protocolo TCP de comunicación */
int req_parse(req_t *req, char *buffer, int size);
int res_parse(res_t *res, char **buffer, int *size);

int data_send();
int data_recv();

#endif
