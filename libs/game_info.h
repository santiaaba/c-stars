#ifndef	GAME_INFO_H
#define	GAME_INFO_H
typedef struct{
	int score;				// Puntaje adquirido
	int state;				// Estado del juego
	int level;				// nivel actual
	int level_status;		// BEGIN | PLAYING | END
	int enemie1;			// Cantidad de enemigos clase1 eliminados
	int enemie2;			// Cantidad de enemigos clase2 eliminados
	int enemie3;			// Cantidad de enemigos clase3 eliminados
} game_info_t;
#endif
