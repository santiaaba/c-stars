/* Encargado de recibir y responder a las peticiones
   enviadas por el cliente */

typedef struct t_command_protocol{
} command_protocol_t

void recive(){
	switch {
		case START:
			sem_wait(&mutex)
			/* START: Solicita iniciar o comenzar el juego */
			sem_post(&mutex)
		case PAUSE:
			sem_wait(&mutex)
			/* PAUSE: Solicita pausar el juego */
			sem_post(&mutex)
		case ALIVE:
			/* ALIVE: se informa que el cliente esta vivo */
		case END:
			sem_wait(&mutex)
			/* END: Solicita finalizar el juego */
			sem_post(&mutex)
		case STATUS:
			sem_wait(&mutex)
			/* STATUS: Solicita retorna el estado del juego */
			sem_post(&mutex)
	}
}
