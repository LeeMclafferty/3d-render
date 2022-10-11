#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL.h>

#include "display.h"
#include "vector.h"
#include "mesh.h"
#include "array.h"

triangle* tris_to_render = NULL;

bool is_running = false;
Uint32 previous_frame_time = 0;

vec3 camera_pos = { 0, 0, -5 };
vec3 cube_rotation = { .x = 0, .y = 0, .z = 0 };
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

	 //Loop over all projected tris and render
	int num_tris = array_length(tris_to_render);
	for (int i = 0; i < num_tris; i++)
	{
		triangle tri = tris_to_render[i];

		//draw vertex points
		draw_rectangle(tri.points[0].x, tri.points[0].y, 3, 3, 0xFF00FF00);
		draw_rectangle(tri.points[1].x, tri.points[1].y, 3, 3, 0xFF00FF00);
		draw_rectangle(tri.points[2].x, tri.points[2].y, 3, 3, 0xFF00FF00);

		// Connect points with triangles
		draw_triangle(tri.points[0].x, tri.points[0].y, tri.points[1].x, tri.points[1].y, tri.points[2].x, tri.points[2].y, 0xFFFF00FF);
	}

	//Clear tris to render every frame loop
	array_free(tris_to_render);

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
}

void update(void)
{
	/* Wait for frame target time to udate the loop. This is a way to keep FPS consistency on 
	different machines. Below is a manual, but unperformant way. Use a proper delay function.*/
	//while (!SDL_TICKS_PASSED(SDL_GetTicks(), previous_frame_time + FRAME_TARGET_TIME));
	SDL_Delay(FRAME_TARGET_TIME);
	
	previous_frame_time = SDL_GetTicks();

	// Empty tris to render
	tris_to_render = NULL;

	cube_rotation.y += 0.01;
	cube_rotation.x += 0.01;
	cube_rotation.z += 0.01;

	// Loop all triangle faces for mesh
	for (int i = 0; i < N_MESH_FACES; i++)
	{
		face mesh_face = mesh_faces[i];
		
		vec3 face_verts[3] = { mesh_verts[mesh_face.vert_a - 1], mesh_verts[mesh_face.vert_b - 1], mesh_verts[mesh_face.vert_c - 1] };

		triangle projected_tri;
		/// Loop over all verts of current face and apply transform
		for (int j = 0; j < 3; j++)
		{
			vec3 transformed_vert = face_verts[j];

			transformed_vert = vec3_rotate_x(transformed_vert, cube_rotation.x);
			transformed_vert = vec3_rotate_y(transformed_vert, cube_rotation.y);
			transformed_vert = vec3_rotate_z(transformed_vert, cube_rotation.z);

			//Translate away from camera
			transformed_vert.z -= camera_pos.z;

			// Project current vertex
			vec2 projected = project(transformed_vert);

			//Scale and translate to the middle of the screen.
			projected.x += (window_width / 2);
			projected.y += (window_height / 2);

			projected_tri.points[j] = projected;
		}

		//tris_to_render[i] = projected_tri;
		array_push(tris_to_render, projected_tri);
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