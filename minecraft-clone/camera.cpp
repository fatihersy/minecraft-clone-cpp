#include "camera.h"

#include <vector>

glm::mat4 view(1.f);
glm::mat4 projection(1.f);

glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 camera_front = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 camera_right;
glm::vec3 world_up;

float player_width = 1.f;
float player_height = 1.8f;
float player_thickness = 0.3f;

std::vector<float> collision_box = { player_thickness, player_height, player_width };

float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch = 0.0f;
float fov = 60.0f;

float camera_speed = 10.f; // adjust accordingly

int WINDOW_WIDTH;
int WINDOW_HEIGHT;

void initialize_camera(int _width, int _height, float _yaw, float _pitch)
{
	WINDOW_WIDTH = _width;
	WINDOW_HEIGHT = _height;

	camera_pos = glm::vec3(1.f, 18.f, 1.f);
	world_up = glm::vec3(0.f, 1.f, 0.f);
	yaw = _yaw;
	pitch = _pitch;

	update_camera_vectors();
}

camera_metrice_packet camera_update_packet() 
{
	return camera_metrice_packet
	{
		glm::lookAt(camera_pos, camera_pos + camera_front, camera_up),
		glm::perspective(glm::radians(fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f),
		camera_pos,
		camera_front
	};
}

void rotate_camera(float xoffset, float yoffset)
{
	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	update_camera_vectors();
}

void move_camera(camera_movement move_direction, float delta_time)
{
	float velocity = camera_speed * delta_time;
	if (move_direction == FORWARD)
		camera_pos += camera_front * velocity;
	if (move_direction == BACKWARD)
		camera_pos -= camera_front * velocity;
	if (move_direction == LEFT)
		camera_pos -= camera_right * velocity;
	if (move_direction == RIGHT)
		camera_pos += camera_right * velocity;
}

void update_camera_vectors() 
{
	// calculate the new Front vector
	glm::vec3 front = glm::vec3(1.f);
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	camera_front = glm::normalize(front);
	// also re-calculate the Right and Up vector
	camera_right = glm::normalize(glm::cross(camera_front, world_up));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	camera_up = glm::normalize(glm::cross(camera_right, camera_front));
}
