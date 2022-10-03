#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"



bool is_running = false;

#define N_POINTS (9 * 9 * 9)
vec3 cube_points[N_POINTS]; //9 x 9 x 9 
vec2 projected_points[N_POINTS];

vec3 camera_pos = { 0, 0, -5 };
float fov = 650;

void setup(void);
void update(void);
void render(void);
void process_input(void);
vec2 project(vec3 point);

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

void render(void)
{
	draw_grid(0xFF141414);

	// Loop over all projected points and render
	for (int i = 0; i < N_POINTS; i++)
	{
		vec2 projected_point = projected_points[i];
		draw_rectangle(projected_point.x + (window_width / 2), projected_point.y + (window_height / 2), 4, 4, 0xFF00FF);
	}


	render_color_buffer();
	clear_color_buffer(0xFF000000);
	SDL_RenderPresent(renderer);
}

void setup(void)
{
	// Allocate the required memory in bytes to hold the color buffer
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * window_width * window_height);

	// Creating a SDL texture that is used to display the color buffer.
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, window_width, window_height);
	
	// creating the cube array
	int point_count = 0;
	for (float x = -1; x <= 1; x += 0.25) 
	{
		for (float y = -1; y <= 1; y += 0.25) 
		{
			for (float z = -1; z <= 1; z += 0.25) 
			{
				vec3 new_point = { .x = x, .y = y, .z = z };
				cube_points[point_count++] = new_point;
			}
		}
	}
}

void update(void)
{
	for (int i = 0; i < N_POINTS; i++)
	{
		// Get each point
		vec3 point = cube_points[i];

		//Simulate moving points away from camera.
		point.z -= camera_pos.z;

		//convert cubes points to 2d vector from 3d
		vec2 projected_point = project(point);

		// Save the new vectors.
		projected_points[i] = projected_point;
	}
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

vec2 project(vec3 point)
{
	vec2 projected_point = { .x = (point.x * fov) / point.z, .y = (point.y * fov) / point.z };

	return projected_point;
}