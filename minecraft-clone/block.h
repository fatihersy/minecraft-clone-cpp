#pragma once

#include "glm/glm.hpp"

typedef struct neigbors
{
	bool up;
	bool down;
	bool right;
	bool left;
	bool front;
	bool back;
} neigbors;

void initialize_block_resources(unsigned int top, unsigned int left, unsigned int bottom);

void update_block_shader(glm::mat4 view, glm::mat4 projection, glm::vec3 position);

void draw_block(glm::vec3 position, neigbors neigbors);
