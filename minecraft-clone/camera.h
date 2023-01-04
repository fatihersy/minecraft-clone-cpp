#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

typedef enum camera_movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
} camera_movement;

typedef struct camera_metrice_packet 
{
    glm::mat4 view_matrix;
    glm::mat4 projection_matrix;
    glm::vec3 get_camera_position;
} camera_metrice_packet;

void initialize_camera(float _width, float _height, float _yaw = -90, float _pitch = 0.f);

camera_metrice_packet camera_update_packet();

void rotate_camera(float pitch, float yaw);
void move_camera(camera_movement move_direction, float delta_time);

void update_camera_vectors();
