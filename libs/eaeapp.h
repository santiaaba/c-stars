#ifndef EAEAPP_H
#define EAEAPP_H

#include <time.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <inttypes.h>

/* Version minima aceptada */
#define EAEAPP_VERSION		1

/* Para determinar si es video o audio */
#define D_VIDEO	0
#define D_SOUND	1

#define REQ_HEADER_SIZE		8		// En Bytes
#define RES_HEADER_SIZE		8		// En Bytes

#define DATA_HEAD_SIZE		8
#define MAX_DATA_BODY		4		// Cantidad de elementos!!! No Bytes
#define MAX_DATA				40		// bytes

#define DATA_ENTITY_SIZE	8		// cantidad de bytes de un entity

/* Los diferentes mensajes que puede recibir el servidor */
#define C_SIN_RESP		0
#define C_CONNECT_1		1
#define C_CONNECT_2		2
#define C_DISCONNECT		3
#define C_KEEPALIVE		4
#define C_KEY_PRESS		5
#define C_GAME_START		6
#define C_GAME_STOP		7
#define C_GAME_PAUSE		8
#define C_GAME_RESUME	9
#define C_GAME_STATUS	10

/* Tamaño de los body de las preguntas y respuestas */
#define BODY_RES_0				0
#define BODY_RES_VERSION		1
#define BODY_RES_STATUS			8

#define BODY_REQ_0				0
#define BODY_REQ_CONNECT		4
#define BODY_REQ_KP				4

/* Las diferentes respuestas */
#define RES_OK						20
#define RES_ERROR_PORT			40
#define RES_ERROR_VERSION		41
#define RES_INCORRECT			50

#define AUX_FORCESTATUS		0b00000001

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
	uint16_t sound[MAX_DATA_BODY];
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

typedef struct{
   uint32_t score;               // Puntaje adquirido
   uint8_t state;               // Estado del juego
   uint8_t power;               // Energia de la nave
   uint8_t level;                // nivel actual
   uint8_t level_state;          // estado del nivel
} res_info_t;

void req_init(req_t *req);
void res_init(res_t *res);
void req_destroy(req_t *req);
void res_destroy(res_t *res);
void req_fill(req_t *req, uint8_t cod, uint16_t size);
void res_fill(res_t *res, uint8_t cod, uint8_t resp, uint16_t size, uint32_t qid);

void printb(char *buffer, int size);

/* Para el protocolo TCP de comunicación */
void eaeapp_res2char(res_t *res, char *buffer, int *size);
void eaeapp_req2char(req_t *req, char *buffer, int *size);
void eaeapp_char2req(req_t *req, char *buffer);
void eaeapp_char2res(res_t *res, char *buffer);

/* Para el protocolo UDP de datos */
void data_to_buffer(data_t *data, char **buffer, int *size);
void buffer_to_data(data_t *data, char *buffer);

/* Otras funciones auxiliares */
void data_entity_copy(data_render_t *dest, data_render_t *orig);
#endif
