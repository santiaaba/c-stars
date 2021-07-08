/* Encargado de recibir y responder a las peticiones
   enviadas por el cliente */

void command_negociate_udp(protocol_t *protocol){
}

void command_handle(protocol_t *protocol, char *buffer, int size){
	switch {
		case NEGOCIATE:
			/* Negocia el puerto udp para el streaming con el cliente */
			command_negociate_udp();
			break;
		case KEY:
			/* Recibo instrucciones de tecla presionada */
			game_key(game);
			break;
		case STATUS:
			/* Conocer el estado del juego */
			game_status(game);
			break;
		case LEVEL:
			/* Configurar nivel a ser jugado */
			game_set_level(game);
			break;
		case PAUSE:
			/* Solicita pausar el juego */
			game_pause(game);
			break;
		case RESUME:
			/* Solicita continuar con el juego */
			game_start(game);
			break;
		case KEEPALIVE:
			/* ALIVE: se informa que el cliente esta vivo */
			break;
		case GAMEOVER:
			game_over(game);
			break;
		case LEAVE:
			/* END: Solicita finalizar la conexion. */
			game_over(game);
			break;
	}
}
