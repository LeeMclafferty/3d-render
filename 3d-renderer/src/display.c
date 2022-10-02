#include "display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int window_width = 1920;
int window_height = 1080;

//Creates SDL window with renderer.
bool init_window(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING != 0))
	{
		fprintf(stderr, "Error initializing SDL.\n");
		return false;
	}

	// Get monitor full screen resolution
	SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(0, &display_mode);

	//window_width = display_mode.w;
	//window_height = display_mode.h;

	// Passing NULL gets rid of window title, making a borderless screen.
	window = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, SDL_WINDOW_BORDERLESS);

	if (!window)
	{
		fprintf(stderr, "Error createing SDL window.\n");
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	if (!renderer)
	{
		fprintf(stderr, "Error createing SDL renderer.\n");
		return false;
	}

	//SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	return true;
}

void destroy_window(void)
{
	free(color_buffer);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void get_display_info(void)
{
	SDL_DisplayMode display_mode;
	for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i)
	{
		int should_be_zero = SDL_GetCurrentDisplayMode(i, &display_mode);
		if (should_be_zero != 0)
		{
			SDL_Log("Error for for video display #%d: %s", i, SDL_GetError());
		}
		else
		{
			SDL_Log("Display #%d: current display mode is %dx%dpx @ %dHz.", i, display_mode.w, display_mode.h, display_mode.refresh_rate);
		}
	}
}

// Looping over all positions in color buff mem and set it to a color.
void clear_color_buffer(uint32_t color)
{
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			color_buffer[(window_width * y) + x] = color;
		}
	}
}

void render_color_buffer(void)
{
	//Copy color buffer to texture to be rendered.
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(window_width * sizeof(uint32_t)));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}


void draw_grid(uint32_t color1, uint32_t color2)
{
	for (int y = 0; y < window_height; y++)
	{
		for (int x = 0; x < window_width; x++)
		{
			if (y % 10 == 0 || x % 10 == 0)
				color_buffer[(window_width * y) + x] = color1;
			else
				color_buffer[(window_width * y) + x] = color2;
		}
	}
}

void draw_rectangle(int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int current_x = x + i;
			int current_y = y + j;

			color_buffer[(window_width * current_y) + current_x] = color;
		}
	}
}