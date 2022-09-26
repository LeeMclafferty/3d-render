#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

bool init_window(void);
void setup(void);
void update(void);
void render(void);
void process_input(void);
void destroy_window(void);
void clear_color_buffer(uint32_t color);
void render_color_buffer(void);

bool is_running = false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 800;
int window_height = 600;

int main(int argc, char* args[])
{
	 is_running = init_window();

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

//Creates SDL window with renderer.
bool init_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING != 0))
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Passing NULL gets rid of window title, making a borderless screen.
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);

	if (!window)
	{
		fprintf(stderr, "Error createing SDL window.\n");
		return false;
	}
	
	//TODO: Create SDL renderer
	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error createing SDL renderer.\n");
		return false;
	}

	return true;
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

	render_color_buffer();

	clear_color_buffer(0xFFFFFF00);

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

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

// Looping over all positions in color buff mem and set it to a color.
void clear_color_buffer(uint32_t color)
{
	for (int y = 0; y < window_height; y++)
	{
		for (int y = 0; y < window_height; y++) 
		{
			for (int x = 0; x < window_width; x++) 
			{
				color_buffer[(window_width * y) + x] = color;
			}
		}
	}
}

void render_color_buffer(void)
{
	//Copy color buffer to texture to be rendered.
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
