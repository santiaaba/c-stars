typedef enum {
	INIT,		/* Menu de inicio */
	READY,	/* Preparado para arrancar el nivel */
	PLAY,		/* Jugando el nivel */
	PAUSE,	/* Nivel pausado */
	END		/* Nivel terminado */
}	status_t;

typedef struct Game{
	status_t status;
	ship_t *player;
	uint32_t score;
	list_t *enemies;
	list_t *shot_enemies;
	list_t *shot_player;
	int fd_render;
	int fd_command;
	int fd_data;
} game_t

game_t *game_init();
void game_bind(game_t *game);
void game_start(game_t *game);
