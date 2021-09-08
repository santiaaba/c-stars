#include "eaeapp_server.h"

void static req_connect_step_one(game_t *g, req_t *req, res_t *res){
	/* Conecta el cliente con el server. Paso 1:
	 * - Verifica la version del cliente.
	 * - Verificamos si aceptamos el puerto udp. */
	  
	res->header.cod = 0;

	// Verificamos que el estado sea WAIT_CONNECT
	if(game_get_state(g) != G_WAIT_CONNECT){
		res->header.resp = RES_INCORRECT;		/* Mensaje no aceptado */
		res->header.size = 0;
		return;
	} 

	// Verificamos version
	if(((req_connect_t*)(req->body))->version < VERSION){
		res->header.resp = RES_ERROR_VERSION;		/* version no aceptada */
		res->header.size = 1;
		res->body = (int*)malloc(sizeof(int));
		*(int*)(res->body) = VERSION;
		return;
	}

	// Verificamos que el udp sea aceptada

	// Tomamos el puerto udp ofrecido por el cliente

	game_set_state(g,G_CONNECT_STEP_ONE);

	// Armamos la respuesta afirmativa
	res->header.resp = RES_OK;
	res->header.size = 0;
}

void static req_connect_step_two(game_t *g, req_t *req, res_t *res){
	/* Conecta el cliente con el server. Paso 2:
    * - Reconfirmamos el puerto udp ofrecido por el cliente. */

	res->header.cod = 0;

	// Verificamos que el estado actual sea CONNECT_STEP_ONE
	if(game_get_state(g) != G_CONNECT_STEP_ONE){
		res->header.resp = RES_INCORRECT;
		res->header.size = 0;
		return;
	}

	game_set_state(g,G_READY);

	// Armamos la respuesta
	res->header.resp = RES_OK;
	res->header.size = 0;
	res->body = NULL;
}

void static req_disconnect(game_t *g, req_t *req, res_t *res){
	/* Desconecta el cliente del server e informa al cliente*/
}

void static req_key_press(game_t *g, req_t *req, res_t *res){

	game_event_t *event;

	if(game_get_state(g) != G_PLAYING){
		res->header.resp = RES_INCORRECT;
		return;
	}

	event = (game_event_t*)malloc(sizeof(game_event_t));
	event->key = ((req_key_press_t*)(req->body))->key;
	event->key_type = ((req_key_press_t*)(req->body))->mode;
	game_event_add(g,event);
}

void static req_game_start(game_t *g, req_t *req, res_t *res){
	/* Inicia el nivel indicado. */
	if(game_get_state(g) != G_READY){
		res->header.resp = RES_INCORRECT;
		return;
	}
	game_start(g,(req_level_t)(req->body));
	//game_set_state(g,G_PLAYING);
}

void static req_game_stop(game_t *g, req_t *req, res_t *res){
	if(game_get_state(g) != G_PAUSE ||
		game_get_state(g) != G_PLAYING){
			res->header.resp = RES_INCORRECT;
			return;
	}
	game_set_state(g,G_STOP);
}

void static req_game_pause(game_t *g, req_t *req, res_t *res){
	if(game_get_state(g) != G_PLAYING){
		res->header.resp = RES_INCORRECT;
		return;
	}
	game_set_state(g,G_PAUSE);
}

void static req_game_resume(game_t *g, req_t *req, res_t *res){
	if(game_get_state(g) != G_PAUSE){
		res->header.resp = RES_INCORRECT;
		return;
	}
	game_set_state(g,G_PLAYING);
}

void static req_game_status(game_t *g, req_t *req, res_t *res){
	// Armamos la respuesta
}

void static req_keep_alive(game_t *g, req_t *req, res_t *res){
	res->header.cod = RES_OK;
}

void server_protocol_handle(game_t *g,char *req_buffer, int req_size,
									 char *res_buffer, int *res_size){
	/* Se encarga de recibir la consulta del cliente y
	 * retornar en los dos últimos parámetros la respuesta.
	 * El server es el encargado de enviarla al cliente */

	req_t req;
	res_t res;

	req_parse(&req,req_buffer,req_size);
	res.header.cod = req.header.cod;

	switch(req.header.cod) {
		case C_CONNECT_1:
			/* Se conecta el cliente. Paso 1 */
			req_connect_step_one(g,&req,&res);
			break;
		case C_CONNECT_2:
			/* Se conecta el cliente. Paso 2 */
			req_connect_step_two(g,&req,&res);
			break;
		case C_DISCONNECT:
			/* Se desconecta el cliente */
			req_disconnect(g,&req,&res);
			break;
		case C_KEEPALIVE:
			/* Cliente informa al server que esta vivo */
			req_keep_alive(g,&req,&res);
			break;
		case C_GAME_START:
			/* Inicia el nivel indicado */
			req_game_start(g,&req,&res);
			break;
		case C_GAME_STOP:
			/* Detiene el nivel que se esta jugando */
			req_game_stop(g,&req,&res);
			break;
		case C_GAME_PAUSE:
			/* Pausa el juego */
			req_game_pause(g,&req,&res);
			break;
		case C_GAME_RESUME:
			/* Continua el juego pausado */
			req_game_resume(g,&req,&res);
			break;
		case C_GAME_STATUS:
			/* Retorna estado y datos del juego */
			req_game_status(g,&req,&res);
			break;
		case C_KEY_PRESS:
			/* Tecla presionada */
			req_key_press(g,&req,&res);
			break;
		default:
			printf("Error de codigo");
	}

	/* Una vez obtenida la respuesta, la convertimos en res_buffer */
	res_parse(&res,&res_buffer,res_size);
}
