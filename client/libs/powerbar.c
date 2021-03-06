#include "powerbar.h"

void powerbar_init(powerbar_t *bar, SDL_Renderer *renderer){
	bar->texture = IMG_LoadTexture(renderer, "img/power_bar.png");
	bar->cut.h = 21;
	bar->cut.w = 8;
	bar->cut.y = 0;

	bar->dest.h = 12;
	bar->dest.w = 8;
	bar->dest.x = 0;
	bar->dest.y = 0;

	bar->x = 0;
	bar->y = 0;

	bar->max = 0;
	bar->renderer = renderer;
	bar->power = 0;
	bar->cant = 0;
}

void powerbar_set_max(powerbar_t *bar, int max){
	bar->max = max;
}

void powerbar_set_power(powerbar_t *bar, int power){
//	printf("power viejo:%i nuevo:%i\n",bar->power,power);
	if(power != bar->power){
		bar->power = power;
		bar->cant = bar->power / (bar->max / CANT_BARS);
//		printf("power cant: %i\n",bar->cant);
	}
}

void powerbar_set_position(powerbar_t *bar, int x, int y){
	bar->x = x;
	bar->y = y;
}

void powerbar_draw(powerbar_t *bar){
	/* Dibujamos las barritas de izquierda a derecha */
	/* Las primeras 5 son rojas.
	   Las siguientes 8 son amarillas.
	   Las últimas 7 son verder. */

//	printf("Dibujando barra: power:%i - max:%i\n",bar->power,bar->max);

	bar->cut.x=0;
	bar->dest.x = bar->x;
	bar->dest.y = bar->y;
	int i = 0;
	while(i < 5 && i < bar->cant -1){
//		printf("dest:(%i,%i) cut:(%i,%i)\n",
//			bar->dest.x,bar->dest.y,bar->cut.x,bar->cut.y);
		SDL_RenderCopy(bar->renderer,
							bar->texture,
							&(bar->cut),
							&(bar->dest));
		bar->dest.x += 10;
		i++;
	}
	bar->cut.x+=8;
	i = 0;
	while(i < 8 && i < bar->cant -6){
//		printf("dest:(%i,%i) cut:(%i,%i)\n",
//			bar->dest.x,bar->dest.y,bar->cut.x,bar->cut.y);
		SDL_RenderCopy(bar->renderer,
							bar->texture,
							&(bar->cut),
							&(bar->dest));
		bar->dest.x += 10;
		i++;
	}
	bar->cut.x+=8;
	i = 0;
	while(i < 7 && i < bar->cant - 14){
//		printf("dest:(%i,%i) cut:(%i,%i)\n",
//			bar->dest.x,bar->dest.y,bar->cut.x,bar->cut.y);
		SDL_RenderCopy(bar->renderer,
							bar->texture,
							&(bar->cut),
							&(bar->dest));
		bar->dest.x += 10;
		i++;
	}
}

void powerbar_destroy(powerbar_t *bar){
	SDL_DestroyTexture(bar->texture);
}
