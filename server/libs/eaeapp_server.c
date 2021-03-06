#include "eaeapp_server.h"

void static req_connect_step_one(game_t *g, char *ip, req_t *req, res_t *res){
	/* Conecta el cliente con el server. Paso 1 */
	/* Recibir este mensaje implica detener el juego
	   y regresar a foja 0 */
	game_set_state(g,G_WAIT_CONNECT);
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);

	// Verificamos version
	if(((req_connect_t*)(req->body))->version < VERSION){
		res->header.resp = RES_ERROR_VERSION;
		res->header.size = BODY_RES_VERSION;
		res->body = (int*)malloc(sizeof(int));
		*(int*)(res->body) = VERSION;
		return;
	}

	// Verificamos que el udp sea aceptado
	// IMPLEMENTAR
	game_init_udp(g,ip,
		((req_connect_t*)(req->body))->udp);
	game_set_state(g,G_CONNECT_STEP_ONE);
	res->header.resp = RES_OK;
}

void static req_connect_step_two(game_t *g, req_t *req, res_t *res){
	/* Conecta el cliente con el server. Paso 2:
    * - Cliente nos reconfirma el udp. */

	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);

	if(game_get_state(g) != G_CONNECT_STEP_ONE){
		res->header.resp = RES_INCORRECT;
		return;
	}

	game_set_state(g,G_READY);
	res->header.resp = RES_OK;
}

void static req_disconnect(game_t *g, req_t *req, res_t *res){
	/* Desconecta el cliente del server e informa al cliente*/
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);
	game_set_state(g,G_WAIT_CONNECT);
	res->header.resp = RES_OK;
}

void static req_key_press(game_t *g, req_t *req, res_t *res){

	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);
	if(game_get_state(g) != G_PLAYING){
		res->header.resp = RES_INCORRECT;
		return;
	}
	game_event_add(g,((req_kp_t*)(req->body))->key,
					((req_kp_t*)(req->body))->action);
	res->header.resp = RES_OK;
}

void static req_game_start(game_t *g, req_t *req, res_t *res){
	/* Inicia el juego. */
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);
	if(game_get_state(g) != G_READY && game_get_state(g) != G_OVER){
		res->header.resp = RES_INCORRECT;
		return;
	}
	res->header.resp = RES_OK;
	game_start(g);
}

void static req_game_stop(game_t *g, req_t *req, res_t *res){
	/* Finaliza el juego por solicitud del cliente. El juego
		queda preparado para una nueva partida */
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);
	if(game_get_state(g) != G_PAUSE &&
		game_get_state(g) != G_PLAYING &&
		game_get_state(g) != G_OVER){
			res->header.resp = RES_INCORRECT;
			return;
	}
	res->header.resp = RES_OK;
	game_set_state(g,G_READY);
}

void static req_game_pause(game_t *g, req_t *req, res_t *res){
	/* Pausa el juego en el nivel en el que se encuentra */
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);
	if(game_get_state(g) != G_PLAYING){
		res->header.resp = RES_INCORRECT;
		return;
	}
	res->header.resp = RES_OK;
	game_set_state(g,G_PAUSE);
}

void static req_game_resume(game_t *g, req_t *req, res_t *res){
	/* Continua el juego en el nivel en que se encuentra a
		pedido del cliente */
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);

	if(game_get_state(g) != G_PAUSE){
		res->header.resp = RES_INCORRECT;
		return;
	}
	res->header.resp = RES_OK;
	game_resume(g);
}

void static req_game_status(game_t *g, req_t *req, res_t *res){
	/* Retorna el estado del nivel y del juego actual */
	res_fill(res,req->header.cod,0,BODY_RES_0,req->header.qid);

	res->body = (game_info_t*)malloc(sizeof(game_info_t));
	res->header.size = BODY_RES_STATUS;
	res->header.resp = RES_OK;
	game_info(g,res->body);
}

void static req_keep_alive(game_t *g, req_t *req, res_t *res){
	res->header.cod = RES_OK;
}

void req_error(req_t *req, res_t *res){
	res->header.resp = RES_INCORRECT;
}

void server_protocol_handle(game_t *g, char *ip, req_t *req, res_t *res){
	/* Se encarga de procesar los mensajes del cliente */

	switch(req->header.cod) {
		case C_CONNECT_1:
			/* Se conecta el cliente. Paso 1 */
			req_connect_step_one((game_t *)g,ip,req,res);
			break;
		case C_CONNECT_2:
			/* Se conecta el cliente. Paso 2 */
			req_connect_step_two((game_t *)g,req,res);
			break;
		case C_DISCONNECT:
			/* Se desconecta el cliente */
			req_disconnect((game_t *)g,req,res);
			break;
		case C_KEEPALIVE:
			/* Cliente informa al server que esta vivo */
			req_keep_alive((game_t *)g,req,res);
			break;
		case C_GAME_START:
			/* Inicia el juego desde el nivel 1 */
			req_game_start((game_t *)g,req,res);
			break;
		case C_GAME_STOP:
			/* Detiene el juego */
			req_game_stop((game_t *)g,req,res);
			break;
		case C_GAME_PAUSE:
			/* Pausa el juego */
			req_game_pause((game_t *)g,req,res);
			break;
		case C_GAME_RESUME:
			/* Continua el juego pausado */
			req_game_resume((game_t *)g,req,res);
			break;
		case C_GAME_STATUS:
			/* Retorna estado y datos del juego */
			req_game_status((game_t *)g,req,res);
			break;
		case C_KEY_PRESS:
			/* Tecla presionada */
			req_key_press((game_t *)g,req,res);
			break;
		default:
			req_error(req,res);
	}
}
