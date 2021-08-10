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
	void *body
} response_t;

/* Para las consultas. Metodos utilizados por el server */
int req_parse(protocol_t *req, char *buffer, int size);
int req_connect(request_t *req);
void req_disconnect(request_t *req, response_t *resp);
void req_keep_alive(request_t *req);
void req_game_start(request_t *req);
void req_game_stop(request_t *req);
void req_game_pause(request_t *req);
void req_game_resume(request_t *req);
void req_game_status(request_t *req);
void req_game_kp(request_t *req);
void req_handle_req(request_t *req);
