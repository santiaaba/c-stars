#ifndef EAEAPP_SERVER_H
#define EAEAPP_SERVER_H

#include "./game.h"
#include "../../libs/eaeapp.h"

#define VERSION 1

void server_protocol_handle(game_t *g, char *req_buffer,
									 int req_size, char *res_buffer,
									 int *res_size);
#endif
