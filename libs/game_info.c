#include "game_info.h"

void game_info2char(char *buffer, game_info_t *g){
	buffer[0] = htonl(g->score);
	buffer[4] = htons(g->state);
	buffer[6] = (char)(g->level);
	buffer[7] = (char)(g->level_state);
}

void game_char2info(char *buffer, game_info_t *g){
	g->score = htonl((uint32_t)buffer[0]);
	g->state = htons((uint16_t)buffer[4]);
	g->level = (uint8_t)buffer[6];
	g->level_state = (uint8_t)buffer[7];
}
