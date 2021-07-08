/* Encargado de recibir y responder a las peticiones
   enviadas por el cliente */

typedef struct t_command_protocol{
} command_protocol_t

void recive(buffer *char ){
	switch {
		case KEY:
			/* Recibo instrucciones de tecla presionada */
		case STATUS:
			/* Conocer el estado del juego */
		case LEVEL:
			/* Configurar nivel a ser jugado */
		case PAUSE:
			sem_wait(&mutex)
			/* PAUSE: Solicita pausar el juego */
			sem_post(&mutex)
		case KEEPALIVE:
			/* ALIVE: se informa que el cliente esta vivo */
		case LEAVE:
			sem_wait(&mutex)
			/* END: Solicita finalizar la conexion */
			sem_post(&mutex)
	}
}
