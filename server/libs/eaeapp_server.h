#ifndef EAEAPP_SERVER_H
#define EAEAPP_SERVER_H

#include "./game.h"
#include "../../libs/eaeapp.h"
#include "../../libs/game_info.h"

#define VERSION 1

void server_protocol_handle(void *g, char *req_buffer,
									 int req_size, char *res_buffer,
									 int *res_size);
#endif
