#ifndef EAEAPP_H
#define EAEAPP_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

/* Version minima aceptada */
#define EAEAPP_VERSION		1

/* Para determinar si es video o audio */
#define D_VIDEO	0
#define D_SOUND	1

#define REQ_HEAD_SIZE		8				// En Bytes
#define RES_HEAD_SIZE		8				// En Bytes
#define DATA_HEAD_SIZE		8
#define MAX_DATA_BODY		200			// En bytes
#define DATA_ENTITY_SIZE	8				// cantidad de bytes de un entity

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

#define RES_OK						20
#define RES_ERROR_PORT			40
#define RES_ERROR_VERSION		41
#define RES_INCORRECT			50

/*********************************************/
/* Estructuas para el envio de datos por UDP */
/*********************************************/
typedef struct {
	uint32_t frame;
	uint8_t type;
	uint16_t size;		/* Cantidad de elementos del body en bytes */
	uint8_t aux;
} data_header_t;

typedef struct {
	uint16_t entity_class;
	int16_t pos_x;
	int16_t pos_y;
	uint8_t sprite;
	uint8_t frame;
} data_render_t;

typedef struct {
	data_header_t header;
	data_render_t body[MAX_DATA_BODY];
} data_t;

/************************************************/
/* Estructuas para el envio de comandos por TCP */
/************************************************/

/***********/
/* Request */
/***********/
typedef struct req_connect {
	uint16_t udp;
	uint16_t version;
} req_connect_t;

typedef struct req_key_press {
	uint16_t key;		// Codigo ASSCI tecla presionada
	uint16_t action;		// Presionada o soltada
} req_kp_t;

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

/* Se permiten bodys customizados. Es el caso de un mensaje
C_GAME_STATUS */

/************/
/* Response */
/************/
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
void eaeapp_req_char2header(req_t *req, char *buffer, int size);
void eaeapp_req_char2body(req_t *req, char *buffer, int size);
void eaeapp_req_header2char(req_t *req, char *buffer, int *size);
void eaeapp_req_body2char(req_t *req, char *buffer, int *size);


void eaeapp_res_char2header(res_t *res, char *buffer, int size);
void eaeapp_res_char2body(res_t *res, char *buffer, int size);
void eaeapp_res_header2char(res_t *res, char *buffer, int *size);
void eaeapp_res_body2char(res_t *res, char *buffer, int *size);




void req_init(req_t *req, uint8_t cod, uint16_t size);
void res_init(res_t *res, uint8_t cod, uint8_t resp, uint16_t size);

void req_to_buffer(req_t *req, char **buffer, int *size);
int buffer_to_req(req_t *req, char *buffer, int size);

void res_to_buffer(res_t *res, char **buffer, int *size,void f(char*,void*));
int buffer_to_res(res_t *res, char *buffer, int size,void f(char*,void*));

/* Para el protocolo UDP de datos */
void data_to_buffer(data_t *data, char **buffer, int *size);
void buffer_to_data(data_t *data, char *buffer, int size);

/* Otras funciones auxiliares */
void data_entity_copy(data_render_t *dest, data_render_t orig);
#endif
