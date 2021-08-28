#ifndef COMMAND_PROTOCL_H
#define COMMAND_PROTOCL_H

#define C_CONNECT			0
#define C_DISCONNECT		1
#define C_KEEPALIVE		2
#define C_KEY_PRESS		3
#define C_GAME_START		4
#define C_GAME_STOP		5
#define C_GAME_PAUSE		6
#define C_GAME_RESUME	7
#define C_GAME_STATUS	8

/* Se encarga de responder a los requerimientos
	que le llegan al server tcp */

/* Para las consultas */

typedef struct req_connect {
	uint16_t udp,
	uint16_t version 
} req_connect_t;

typedef struct req_key_press {
	uint16_t key,
	uint16_t code
} req_key_press_t;

typedef uint32_t req_level_t ;

typedef struct request {
	struct header{
		uint8_t cod,
		uint8_t aux,
		uint16_t size,
		uint32_t qid
	},
	void *body			// Los body varian en base al mensaje
} request_t;

/* Para las respuestas */

typedef struct resp_status {
	uint32_t score
} resp_status_t;

typedef struct resp {
	struct header{
		uint8_t cod,
		uint8_t resp,
		uint16_t size,
		uint32_t qid
	},
	void *body		// Los body varian en base al mensaje
} response_t;

/* Para las consultas. Metodos utilizados por el server para
	procesar los mensajes recibidos */
int req_parse(request_t **req, char *buffer, int size);
void req_handle(request_t *req);

int req_connect(request_t *req);
void req_disconnect(request_t *req, response_t *resp);
void req_keep_alive(request_t *req);
void req_game_start(request_t *req);
void req_game_stop(request_t *req);
void req_game_pause(request_t *req);
void req_game_resume(request_t *req);
void req_game_status(request_t *req);
void req_game_kp(request_t *req);

/* Para las respuestas. Metodos utilizados por el cliente ante
	las respuestas del servidor */
int res_parse(restponse_t **res, char *buffer, int size);
void res_handle(response_t *req);

#endif
