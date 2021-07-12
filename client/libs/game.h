#include "screen.h"

void game_start(game_t *game){
	while(game->status != END){
		switch(game->status){
			case PLAY:
				game_play(game);
				break;
			case CONNECT:
				game_connect(game);
				break;
			case MAINMENU:
				game_main_menu(game);
				break;
			case PAUSE:
				game_pause(game);
				break;
		}
	}
}

void game_play(game_t *game){
	/* Loop cuando se esta jugando. El ritmo lo impone
	   el servidor. El servidor va enviando los datos de
		un frame a medida que los va generando. El cliente
		debe dibujar el frame inmediatamente cuando determina
		que los siguientes datos corresponden al frame siguiente */
	while(game->status == PLAY){
		/* Capturamos los eventos del teclado */

		/* Colocamos los eventos en el buffer. No los enviamos directamente
			por TCP desde aca porque puede generar demora en el render de la
			imagen ya que el send del TCP es bloqueante */
		screen_draw(game->screen);
		nanosleep(20000);		// Aguardamos 20 milisegundos
	}
}

void game_connect(){
	/* Loop cuando se esta esperando conectar
	   contra un servidor */
	while(game->status == CONNECT){
	}
}

void game_main_menu(){
	/* Loop cuando se esta presentando el menu
		principal */
	while(game->status == MENU){
	}
}

void game_pause(){
	/* Loop cuando se esta presentando la pausa */
	while(game->status == PAUSE){
	}
}

void game_make_menu(){
}
