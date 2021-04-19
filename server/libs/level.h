typedef struct Game{
	list_t enemies;
	list_t shoots;
	uint32_t score;
} game_t

game_t *game_init();

void game_resume();
void game_pause();
