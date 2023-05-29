#pragma once

#include "glm/glm.hpp"
#include <vector>

typedef enum structure_enum {
	NONE = 0,
	TREE = 2
} structure_enum;


typedef struct neigbors
{
	bool up;
	bool down;
	bool right;
	bool left;
	bool back;
	bool front;
} neigbors;

typedef struct block
{
	bool is_active;

	neigbors neigbors;

	structure_enum type;

	bool is_buried()
	{
		return
			neigbors.back &&
			neigbors.down &&
			neigbors.front &&
			neigbors.left &&
			neigbors.right &&
			neigbors.up;
	}

}block;

void initialize_block_resources(std::vector<uint16_t> block_textures);

void update_block_shader(glm::mat4 view, glm::mat4 projection, glm::vec3 position);

void draw_block(glm::vec3 position, neigbors neigbors);

void begin_draw();

void end_draw();
