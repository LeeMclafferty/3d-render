#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "display.h"


void setup(void);
void update(void);
void render(void);
void process_input(void);

bool is_running = false;

int main(int argc, char* args[])
{
	 is_running = init_window();

	 get_display_info();
	 setup();

	 while (is_running)
	 {
		 process_input();
		 update();
		 render();
	 }

	 destroy_window();

	return 0;
}

void setup(void)
{
	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
}

void update(void)
{
	//TODO:
}

void render(void)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
	SDL_RenderClear(renderer);

	draw_grid(0xFF000000, 0xFF4b5320);

	draw_rectangle(300, 300, 300, 250, 0xFFFF00FF);

	render_color_buffer();
	clear_color_buffer(0xFF000000);

	SDL_RenderPresent(renderer);
}

void process_input(void)
{
	SDL_Event event;
	SDL_PollEvent(&event);

	switch (event.type)
	{
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) // if Polled event is esacpe the quit.
			is_running = false;
	}
}