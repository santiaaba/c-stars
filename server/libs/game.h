typedef enum {
	INIT,		/* Menu de inicio */
	READY,	/* Preparado para arrancar el nivel */
	PLAY,		/* Jugando el nivel */
	PAUSE,	/* Nivel pausado */
	END		/* Juego terminado */
}	status_t;

typedef struct t_game{
	status_t status;
	uint32_t score;
	ship_t *player;
	lista_t *enemies;
	lista_t *shot_enemies;
	lista_t *shot_player;
} game_t

void game_init(game_t *game);
void game_run(game_t *game);
void game_set_level(game_t *game);
void game_start(game_t *game);
void game_pause(game_t *game);
void game_over(game_t *game);
