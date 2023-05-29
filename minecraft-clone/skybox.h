#pragma once

#include "glm/glm.hpp"

void initialize_skybox_resources(uint16_t texture);

void draw_skybox(glm::mat4 model, glm::mat4 view, glm::mat4 projection, glm::vec3 position);
