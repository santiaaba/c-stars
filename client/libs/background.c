#include "background.h"

void background_init(background_t *bg, SDL_Renderer *renderer,
							SDL_Texture *texture, int mod){
	bg->renderer = renderer;
	bg->texture = texture;
	bg->cut.x = 0;
	bg->cut.y = 0;
	bg->destiny.x = 0;
	bg->destiny.y = 0;

	SDL_QueryTexture(texture, NULL, NULL, &(bg->cut.w),
         &(bg->cut.h));
	bg->destiny.w = bg->cut.w;
	bg->destiny.h = bg->cut.h;
	bg->mod = mod;
}
/*
void background_begin(background_t *bg){
	bg->destiny.x = 0;
	bg->destiny.y = 0;
}
*/
void background_draw(background_t *bg){
//	printf("BG: (%i,%i,%i,%i) | (%i,%i,%i,%i)\n",
//			bg->cut.x,bg->cut.y,bg->cut.w,bg->cut.h,
//			bg->destiny.x,bg->destiny.y,bg->destiny.w,bg->destiny.h);
	SDL_RenderCopy(
		bg->renderer,
		bg->texture,
		&(bg->cut),
		&(bg->destiny)
	);
	if(bg->mod == BG_DINAMIC)
		bg->destiny.x -= BACKGROUND_SPEED;
}
