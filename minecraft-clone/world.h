#pragma once

#include <vector>
#include <string>
#include "glm/glm.hpp"

static std::vector<std::string> skybox_faces =
{
    "D:/Workspace/CLang/Resources/skybox/mc_right.jpg",
    "D:/Workspace/CLang/Resources/skybox/mc_left.jpg",
    "D:/Workspace/CLang/Resources/skybox/mc_up.jpg",
    "D:/Workspace/CLang/Resources/skybox/mc_down.jpg",
    "D:/Workspace/CLang/Resources/skybox/mc_front.jpg",
    "D:/Workspace/CLang/Resources/skybox/mc_back.jpg",
};

void initialize_world(std::vector<uint16_t> block_textures, uint16_t skybox_texture);

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position, glm::vec3 front);

bool is_on_frustum(glm::mat4 view, glm::mat4 projection, glm::vec3 position);

