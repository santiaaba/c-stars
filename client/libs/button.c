#include "button.h"

void static button_text_make(button_t *button){

	SDL_Surface* surface;
	SDL_Color *text_color;
	int texture_w, texture_h;

	if(button->focus)
		text_color = &(button->font_color_focus);
	else
		text_color = &(button->font_color);

   surface = TTF_RenderText_Solid(button->font, button->text, *text_color);
   button->texture = SDL_CreateTextureFromSurface(button->renderer, surface);
   SDL_FreeSurface(surface);
   SDL_QueryTexture(button->texture, NULL,NULL, &texture_w, &texture_h);
}

void button_init(button_t *button, int x, int y, int w,
						int h, int focus, SDL_Renderer *renderer){

	button->focus = focus;
	button->dest.x = x;
	button->dest.y = y;
	button->dest.w = w;
	button->dest.h = h;
	button->renderer = renderer;
	button->font = TTF_OpenFont(FONT, BUTTON_FONT_SIZE);
	if(!button->font)
      printf("TTF_OpenFont: %s\n", TTF_GetError());
	button->text = "";
	button->texture = NULL;
}

void button_text(button_t *button, char *text){
	button->text=(char*)realloc(button->text,strlen(text) + 1);
	strcpy(button->text,text);
	button_text_make(button);
}

void button_bg_color(button_t *button,
                     uint8_t r,uint8_t g,uint8_t b,
                     uint8_t focus_r,uint8_t focus_g,
                     uint8_t focus_b){
	button->bg_color.r = r;
	button->bg_color.g = g;
	button->bg_color.b = b;

	button->bg_color_focus.r = focus_r;
	button->bg_color_focus.g = focus_g;
	button->bg_color_focus.b = focus_b;
}

void button_font_color(button_t *button,
                       uint8_t r,uint8_t g,uint8_t b,
                       uint8_t focus_r,uint8_t focus_g,
                       uint8_t focus_b){
	button->font_color.r = r;
	button->font_color.g = g;
	button->font_color.b = b;

	button->font_color_focus.r = focus_r;
	button->font_color_focus.g = focus_g;
	button->font_color_focus.b = focus_b;
}

void button_border_color(button_t *button,
                         uint8_t r,uint8_t g,uint8_t b,
                         uint8_t focus_r,uint8_t focus_g,
                         uint8_t focus_b){
	button->border_color.r = r;
	button->border_color.g = g;
	button->border_color.b = b;

	button->border_color_focus.r = focus_r;
	button->border_color_focus.g = focus_g;
	button->border_color_focus.b = focus_b;
}

void button_draw(button_t *button){

	
	printf("Dibujando el fondo\n");
	if(button->focus)
		SDL_SetRenderDrawColor(button->renderer,
                   button->bg_color_focus.r,
                   button->bg_color_focus.g,
                   button->bg_color_focus.b,
                   255);
	else
		SDL_SetRenderDrawColor(button->renderer,
                   button->bg_color.r,
                   button->bg_color.g,
                   button->bg_color.b,
                   255);
	SDL_RenderFillRect(button->renderer,&(button->dest));

	printf("Dibujamos el borde\n");
	if(button->focus)
		SDL_SetRenderDrawColor(button->renderer,
                   button->border_color_focus.r,
                   button->border_color_focus.g,
                   button->border_color_focus.b,
                   255);
	else
		SDL_SetRenderDrawColor(button->renderer,
                   button->border_color.r,
                   button->border_color.g,
                   button->border_color.b,
                   255);

	SDL_RenderDrawRect(button->renderer,&(button->dest));

	printf("Dibujamos el texto");
	SDL_RenderCopy(button->renderer,button->texture,NULL,&(button->dest));
}

void button_focus(button_t *button){
	button->focus = 1;
	button_text_make(button);
}

void button_exit(button_t *button){
	button->focus = 0;
	button_text_make(button);
}
