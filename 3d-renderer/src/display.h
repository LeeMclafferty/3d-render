#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

extern int window_width;
extern int window_height;


bool init_window(void);
void destroy_window(void);

void get_display_info(void);

void clear_color_buffer(uint32_t color);
void render_color_buffer(void);

void draw_grid(uint32_t color1, uint32_t color2);
void draw_rectangle(int x, int y, int width, int height, uint32_t color);