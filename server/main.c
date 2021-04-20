#include "libs/point.h"
#include "libs/ship.h"
#include "libs/shoot.h"
#include "libs/game.h"
#include <sys/socket.h>

void main(void *args){
	game_t *game;

	while(true){
		/* Levantamos el puerto de conexion */
		/* Bindeamos el socket */
		/* Aguardamos una conexión */

		/* Negociamos los puertos del render, comandos y datos */

		game = game_init()

		/* Pasamos los socket al juego */
		game_bind(game,fd_render,fd_command,fd_data)

		/* Iniciamos el juego */
		game_start(game)
	}
}
