#pragma once

#include <vector>
#include <string>
#include "glm/glm.hpp"

static std::vector<std::string> skybox_faces =
{
    "D:/Workspace/skybox/right.jpg",
    "D:/Workspace/skybox/left.jpg",
    "D:/Workspace/skybox/top.jpg",
    "D:/Workspace/skybox/bottom.jpg",
    "D:/Workspace/skybox/front.jpg",
    "D:/Workspace/skybox/back.jpg",
};

void initialize_world(unsigned int skybox_texture);

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position);

