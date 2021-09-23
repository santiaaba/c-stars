#ifndef	GAME_INFO_H
#define	GAME_INFO_H

#include <arpa/inet.h>
#include <stdint.h>

typedef struct{
	uint32_t score;					// Puntaje adquirido
	uint16_t state;					// Estado del juego
	uint8_t level;						// nivel actual
	uint8_t level_state;				// estado del nivel
} game_info_t;

void game_info2char(char *buffer, game_info_t *g);
void game_char2info(char *buffer, game_info_t *g);
#endif
