/* Encargado de recibir y responder a las peticiones
   enviadas por el cliente */

int command_parse(protocol_t *req, char *buffer, int size){
	/* Parsea los datos del buffer a la estructura request_t.
		Retorna 1 si tuvo exito. Retorna 0 si no lo tuvo. */

	buffer[0]	// El primer char conforma el codigo y el aux.
					// ambos de 8 bits aca uno.
	buffer[1]	// El siguiente char es el size del data
	buffer[2]	// Es el qid entregado por el cliente
	
	// Verificar que el primer char este en rango
	// Verificar que el siguiente char este en rango
	// Verificar que el tercer char este en rango

	// Armar el encabezado

	//Dependiendo del codigo... armar el body a partir del 4to byte
}

int command_connect(request_t *req){
	/* Conecta el cliente con el server. Retorna 1 si
	   tuvo exito. Retorna 0 si no lo tuvo. En ambos casos
		informa al cliente */
}

void command_disconnect(request_t *req, response_t *resp){
	/* Desconecta el cliente del server e informa al cliente*/
}

void command_keep_alive(request_t *req){
	/* Recibe el keepalive del cliente */
}

void command_game_start(request_t *req){
	/* Inicia el nivel indicado. */
}

void command_game_stop(request_t *req){
}

void command_game_pause(request_t *req){
}

void command_game_resume(request_t *req){
}

void command_game_status(request_t *req){
}

void command_game_kp(request_t *req){
}

void command_handle_req(request_t *req){
	switch(req->header.cod) {
		case C_CONNECT:
			/* Se conecta el cliente */
			command_connect(req);
			break;
		case C_DISCONNECT:
			/* Se desconecta el cliente */
			command_disconnect(req);
			break;
		case C_KEEPALIVE:
			/* Cliente informa al server que esta vivo */
			command_keep_alive(req);
			break;
		case C_GAME_START:
			/* Inicia el nivel indicado */
			command_game_start(req);
			break;
		case C_GAME_STOP:
			/* Detiene el nivel que se esta jugando */
			command_game_stop(req);
			break;
		case C_GAME_PAUSE:
			/* Pausa el juego */
			command_game_pause(req);
			break;
		case C_GAME_RESUME:
			/* Continua el juego pausado */
			command_game_resume(req);
			break;
		case C_GAME_STATUS:
			/* Retorna estado y datos del juego */
			command_game_status(req);
			break;
		case C_KEY_PRESS:
			/* Tecla presionada */
			command_game_kp(req);
			break;
		case default:
			printf("Error de codigo");
	}
}
