#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} camera_movement;

void initialize_camera(float _width, float _height, float _yaw, float _pitch);

glm::mat4 get_view_matrix();
glm::mat4 get_projection_matrix();
glm::vec3 get_camera_position();

void rotate_camera(float pitch, float yaw);
void move_camera(camera_movement move_direction, float delta_time);

void update_camera_vectors();
