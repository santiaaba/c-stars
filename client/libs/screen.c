void screen_init(screen_t *screen){
	screen->play_groud = NULL;
	screen->entities = (lista_t *)malloc(sizeof(lista_t));
	lista_init(screen->entities,sizeof(entity_t));
}

void screen_window(screen_t *screen, SQL_Window *window){
	screen->screen = SQL_GetWindowSurface(window);
}

void screen_draw(screen_t *screen){
	entity_t *entity = NULL;
	lista_first(screen->entities);
	while(!lista_eol(screen->entities)){
		entity = lista_get(screen->entities);
		entity_draw(entity,screen->play_groud)
		lista_next(screen->entities);
	}
}

lista_t *screen_get_entities(screen_t *screen){
}

void screen_destroy(screent_t **screen){
	lista_clean(*screen->lista);
}
