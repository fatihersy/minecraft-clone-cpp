#pragma once

#include <vector>
#include <string>
#include "glm/glm.hpp"

static std::vector<std::string> skybox_faces =
{
    "D:/Workspace/CLang/Resources/skybox/right.jpg",
    "D:/Workspace/CLang/Resources/skybox/left.jpg",
    "D:/Workspace/CLang/Resources/skybox/top.jpg",
    "D:/Workspace/CLang/Resources/skybox/bottom.jpg",
    "D:/Workspace/CLang/Resources/skybox/front.jpg",
    "D:/Workspace/CLang/Resources/skybox/back.jpg",
};

void initialize_world(std::vector<unsigned int> block_textures, unsigned int skybox_texture);

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position, glm::vec3 front);

bool is_on_frustum(glm::mat4 view, glm::mat4 projection, glm::vec3 position);

