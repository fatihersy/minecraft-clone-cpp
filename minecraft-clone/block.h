#pragma once

#include "glm/glm.hpp"
#include <vector>

typedef struct neigbors
{
	bool up;
	bool down;
	bool right;
	bool left;
	bool front;
	bool back;
} neigbors;

void initialize_block_resources(std::vector<unsigned int> block_textures);

void update_block_shader(glm::mat4 view, glm::mat4 projection, glm::vec3 position);

void draw_block(glm::vec3 position, neigbors neigbors);

void begin_draw();

void end_draw();
