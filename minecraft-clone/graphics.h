#pragma once

int create_window(const char* title, int width, int height, void* update_input, void* mouse_callback, void* mouse_button_callback);

void terminate_window();

void graphics_begin_render();

void graphics_update_render();

void graphics_end_render();

void* get_window_instance();

unsigned int get_window_width();
unsigned int get_window_height();


