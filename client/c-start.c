#include <stdint.h>
#include <stdlib.h>
#include "screen.h"

int main(char argc, char * argv[]){
	
	screen_t *screen;

	if (screen_init(screen) < 0){
		exit(1);
	}

	/* Creamos el hilo que se encarga del servidor UDP que recibe los datos */
	if(0 != pthread_create(&t_render, NULL, &render_server_start, s)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		exit(2);
	}

	/* Creamos el hilo que se encarga de mostrar el screen */
	if(0 != pthread_create(&t_screen, NULL, &render_server_start, s)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		exit(2);
	}

	/* Finalizamos el programa */
	screen_destroy(screen);
	return 0;
}
