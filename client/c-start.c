#include <stdint.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#define SCREEN_WIDTH		1024
#define SCREEN_HEIGHT	600
#define SCREEN_BPP		24

int main(char argc, char * argv[]){
	
	screen_t *screen;
	SDL_windows *window = NULL;

	if(SDL_init(SDL_INIT_VIDEO) < 0){
		printf("No fue posible iniciar el video. ");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1
	}

	windows = SDL_CreateWindow("C-Start", 20, 20, SCREEN_WIDTH,
										SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if(windows == NULL){
		printf("No fue posible crear la ventana. ");
		printf("Error SDL: %s\n", SDL_GetError());
		return 1
	}

	screen_init(screen);
	screen_window(screen,SQL_GetWindowSurface(window));

	/* Creamos el hilo que se encarga del servidor de comandos */
	if(0 != pthread_create(&t_command, NULL, &command_server_start, s)){
		printf("Error al querer crear el hilo para el servidor de comandos\n");
		exit(2);
	}

	/* Creamos el hilo que se encarga de la logica del juego */
	if(0 != pthread_create(&(s->do_task), NULL, &game_run, &game)){
		printf ("Imposible levantar el hilo para realizar tareas\n");
		exit(2);
	}

	/* Finalizamos el programa */
	screen_destroy(screen);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
