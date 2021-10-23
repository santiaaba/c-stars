#ifndef UDP_SERVER_H
#define UDP_SERVER_H

int udp_server_init(udp_server_t *s){

	if ( (s->sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
		perror("socket creation failed");
		return 0;
	}

	memset(&(s->servaddr), 0, sizeof(s->servaddr));
	memset(&(s->cliaddr), 0, sizeof(s->cliaddr));

	s->servaddr.sin_family	 = AF_INET; // IPv4
	s->servaddr.sin_addr.s_addr = INADDR_ANY;
	s->servaddr.sin_port = htons(PORT);

	if ( bind(s->sockfd, (const struct sockaddr *)&(s->servaddr),
		sizeof(s->servaddr)) < 0 ){
			perror("bind failed");
			return 0;
	}
}

void udp_server_start(udp_server_t *s){

	int len, n;
	data_t *data;
  
	len = sizeof(s->cliaddr);

	while(game_get_status(s->game) != ){
		n = recvfrom(s->sockfd, (char *)buffer, MAXLINE, 
				MSG_WAITALL, ( struct sockaddr *) &(s->cliaddr),
				&len);

		buffer_to_data(buffer,data);
		/* Si la data corresponde a un frame nuevo entonces
			dibujamos la pantalla. */
		if(data->header.frame != g->screen_frame && g->screen_frame != 0){
			g->screen_frame = data->header.frame;
			SDL_RenderPresent(g->renderer);
			SDL_RenderClear(g->renderer);
		}
		for(int i=0;i<data->header.size;i++){
			/* Rectangulo para dibujar en pantalla */
			position.x = data->body[j].pos_x;
			position.y = data->body[j].pos_y;
			position.w = g->entities[data->body[j].entity_class].w;
			position.h = g->entities[data->body[j].entity_class].h;

			/* Rectangulo para recortar la textura */
			frame.w = position.w;
			frame.h = position.h;
			frame.x = frame.h * data->body[j].sprite;
			frame.y = frame.w * data->body[j].frame;

			/* Dibujamos */
			SDL_RenderCopy(
				g->renderer,
				g->entities[data->body[j].entity_class].texture,
				&frame,
				&position);
		}
	}
}
#endif
