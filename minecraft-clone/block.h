#pragma once

#include "glm/glm.hpp"

void initialize_block_resources();

void update_block_shader(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 position);

void draw_block(unsigned int texture);
