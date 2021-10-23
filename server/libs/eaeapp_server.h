#ifndef EAEAPP_SERVER_H
#define EAEAPP_SERVER_H

#include "./game.h"
#include "../../libs/eaeapp.h"
#include "../../libs/game_info.h"

#define VERSION 1

void server_protocol_handle(game_t *g, char *ip, req_t *req, res_t *res);

#endif
