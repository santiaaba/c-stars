#ifndef	GAME_INFO_H
#define	GAME_INFO_H
typedef struct{
	int score;					// Puntaje adquirido
	int state;					// Estado del juego
	uint8_t level;				// nivel actual
	uint8_t level_state;
} game_info_t;
#endif
