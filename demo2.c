#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <unistd.h>

/* Moving Rectangle */
int main(int argc, char *argv[])
{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Texture *ship;
	SDL_Event event;
	SDL_Rect ship_frame;
	SDL_Rect dest;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Couldn't initialize SDL: %s", SDL_GetError());
		return 3;
	}

	window = SDL_CreateWindow("SDL_CreateTexture",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			1024, 768,
			SDL_WINDOW_RESIZABLE);

	renderer = SDL_CreateRenderer(window, -1, 0);

	//texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 1024, 768);

	/* Cargamos la imagen */
	ship = IMG_LoadTexture(renderer, "ship.bmp");

	/* Creamos el rectangulo frame */
	ship_frame.x = 69;
	ship_frame.y = 89;
	ship_frame.w = 87;
	ship_frame.h = 80;

	/* Creamos el rectangulo de destino */
	dest.x = 100;
	dest.y = 100;
	dest.w = 87;
	dest.h = 80;

	while (1) {
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			break;
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer,ship,&ship_frame,&dest);
		SDL_RenderPresent(renderer);
		sleep(100);
	}
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
