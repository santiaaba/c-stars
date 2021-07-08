/* Se encarga de responder a los requerimientos
	que le llegan al server tcp */

typedef struct {
	t_game *game;
} protocol_t

void protocol_init(protocol_t *protocol, game_t *game);

void protocol_negociate_udp(protocol_t *protocol);

void protocol_handle();
