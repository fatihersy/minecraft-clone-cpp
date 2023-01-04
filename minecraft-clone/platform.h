#pragma once

#include <vector>
#include <string>

#include "camera.h"

unsigned int loadTexture(std::string path, bool is_png);
unsigned int loadCubemap(std::vector<std::string> path);

void initialize_platform(const char* window_title, float _window_width, float _window_height);
void shutdown_platform();

void platform_begin_frame();
void platform_end_frame();

bool platform_pump_messages();
