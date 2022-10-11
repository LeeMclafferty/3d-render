#include "display.h"
#include <math.h>

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


void draw_grid(uint32_t color)
{
	for (int y = 0; y < window_height; y += 10) 
	{
		for (int x = 0; x < window_width; x += 10) 
		{
			color_buffer[(window_width * y) + x] = color;
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

			draw_pixel(current_x, current_y, color);
		}
	}
}

/*
	With this color buffer, the pixels are being represent in memory as a 1d array. 
	So your screen is not 1920x1080 to the array, it is a big 2,073,600 row of uint32_t (your pixels).
	you can find pixels using 2D cords with this equation. So if you pass in x = 30 y = 20, then you will
	get to index 38,430 for that pixel. The equation would look like ((1920 * 20) + 30).
*/
void draw_pixel(int x, int y, uint32_t color)
{
	if(x >= 0 && x < window_width && y >= 0 && y < window_height)
		color_buffer[(window_width * y) + x] = color;
}

//DDA line drawing algorithm
void draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
	// Change in x and y (rise/run)
	int delta_x = x1 - x0;
	int delta_y = y1 - y0;

	// save the length of longest side (x or y)
	int longest_side_len = abs(delta_x) >= abs(delta_y) ? abs(delta_x) : abs(delta_y); 

	//One value will always eval to 1
	float x_inc = delta_x / (float)longest_side_len;
	float y_inc = delta_y / (float)longest_side_len;

	float current_x = x0;
	float current_y = y0;
	// Draw a pixel on the current pixel, then increment to the next.
	for (int i = 0; i <= longest_side_len; i++)
	{
		draw_pixel(round(current_x), round(current_y), color);
		current_x += x_inc;
		current_y += y_inc;
	}

}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	draw_line(x0, y0, x1, y1, color);
	draw_line(x1, y1, x2, y2, color);
	draw_line(x2, y2, x0, y0, color);
}