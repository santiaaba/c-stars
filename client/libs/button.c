#include "button.h"

void static button_text_make(button_t *button){

	SDL_Surface* surface;
	SDL_Color *text_color;
	SDL_Rect *b_dest;
	SDL_Rect *t_dest;

	b_dest = &(button->dest);
	t_dest = &(button->texture_dest);

	if(button->focus)
		text_color = &(button->font_color_focus);
	else
		text_color = &(button->font_color);

   surface = TTF_RenderText_Solid(button->font, button->text, *text_color);
   button->texture = SDL_CreateTextureFromSurface(button->renderer, surface);
   SDL_FreeSurface(surface);
   SDL_QueryTexture(button->texture, NULL, NULL, &(button->texture_dest.w),
			&(button->texture_dest.h));

	t_dest->x = b_dest->x + (b_dest->w - t_dest->w)/2;
	t_dest->y = b_dest->y + (b_dest->h - t_dest->h)/2;
}

void button_init(button_t *button, int x, int y, int w,
						int h, int focus, SDL_Renderer *renderer){

	button->focus = focus;

	button->dest.x = x;
	button->dest.y = y;
	button->dest.w = w;
	button->dest.h = h;

	button->bg_color.r = button->bg_color_focus.r = 255;
	button->bg_color.g = button->bg_color_focus.g = 255;
	button->bg_color.b = button->bg_color_focus.b = 255;

	button->font_color.r = button->font_color_focus.r = 0;
	button->font_color.g = button->font_color_focus.g = 0;
	button->font_color.b = button->font_color_focus.b = 0;

	button->border_color.r = button->border_color_focus.r = 100;
	button->border_color.g = button->border_color_focus.g = 100;
	button->border_color.b = button->border_color_focus.b = 100;

	button->renderer = renderer;
	button->font = TTF_OpenFont(FONT, BUTTON_FONT_SIZE);
	if(!button->font)
      printf("TTF_OpenFont: %s\n", TTF_GetError());
	button->text = NULL;
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
	uint8_t r,g,b,a;
	SDL_GetRenderDrawColor(button->renderer,&r,&g,&b,&a);

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
	SDL_RenderCopy(button->renderer,button->texture,NULL,&(button->texture_dest));
	//Restauramos color
	SDL_SetRenderDrawColor(button->renderer,r,g,b,255);
}

void button_focus(button_t *button){
	button->focus = 1;
	button_text_make(button);
}

void button_exit(button_t *button){
	button->focus = 0;
	button_text_make(button);
}

void button_destroy(button_t *button){
	SDL_DestroyTexture(button->texture);
	free(button->text);
}
