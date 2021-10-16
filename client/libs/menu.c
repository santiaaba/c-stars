/* Menu */

#include "menu.h"

void menu_init(menu_t *menu, int x, int y, SDL_Renderer *renderer){ 
	menu->dest.x = x;
	menu->dest.y = y;
	menu->index = 0;
	menu->size = 0;
	menu->renderer = renderer;
	menu->show = 1;
	menu->looked = 0;
}

void menu_add(menu_t *menu, char *label){
	int i = menu->size;
	int f = menu->size == 0;
	int x = menu->dest.x;
	int y = menu->dest.y + (i * (M_BUTTON_H + 20));
	
	menu->index = f;

	menu->options[i] = (button_t*)malloc(sizeof(button_t));
	button_init(menu->options[i],x,y,M_BUTTON_W,M_BUTTON_H,f,menu->renderer);

	button_bg_color(menu->options[i],
		B_BG_R, B_BG_G, B_BG_B,
		B_BG_F_R, B_BG_F_G, B_BG_F_B);

   button_font_color(menu->options[i],
		B_TEXT_R, B_TEXT_G, B_TEXT_B,
		B_TEXT_F_R, B_TEXT_F_G, B_TEXT_F_B);

   button_border_color(menu->options[i],
		B_BORDE_R, B_BORDE_G, B_BORDE_B, 
		B_BORDE_F_R, B_BORDE_F_G, B_BORDE_F_B);

   button_text(menu->options[i],label);

	menu->size++;
}

void static menu_focus(menu_t *menu){
	for(int i=0; i<menu->size; i++){
		if(i==menu->index)
			button_focus(menu->options[i]);
		else
			button_exit(menu->options[i]);
	}
}

void menu_down(menu_t *menu){
	if(menu->index < menu->size - 1 &&
		!menu->looked){
		menu->index++;
		menu_focus(menu);
	}
}

void menu_up(menu_t *menu){
	if(menu->index > 0 && !menu->looked){
		menu->index--;
		menu_focus(menu);
	}
}

int menu_is_looked(menu_t *menu){
	return menu->looked;
}

void menu_unlooked(menu_t *menu){
	menu->looked = 0;
}

void menu_looked(menu_t *menu){
	menu->looked = 1;
}

int menu_get_index(menu_t *menu){
	return menu->index;
}

void menu_show(menu_t *menu){
	menu->show = 1;
}

void menu_hidden(menu_t *menu){
	menu->show = 0;
}

void menu_draw(menu_t *menu){
	if(menu->show)
		for(int i=0;i<menu->size;i++)
			button_draw(menu->options[i]);
}
void menu_destroy(menu_t *menu){
	for(int i=0;i<menu->size;i++){
		button_destroy(menu->options[i]);
		free(menu->options[i]);
	}
}
