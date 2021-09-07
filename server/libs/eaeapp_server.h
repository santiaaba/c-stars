#ifndef EAEAPP_SERVER_H
#define EAEAPP_SERVER_H

#include "./game.h"
#include "../../libs/eaeapp.h"

void server_protocol_handle(game_t *g, char *req_buffer,
									 int req_size, char *res_buffer,
									 int *res_size);
#endif
