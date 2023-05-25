#include "world.h"

#include "block.h"
#include "skybox.h"
#include "fdefines.h"
#include "PerlinNoise.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>

// world space positions of our cubes
#define BOUND_X 256
#define BOUND_Y 20
#define BOUND_Z 256

#define MAX_WORLD_X 256
#define MAX_WORLD_Y 16
#define MAX_WORLD_Z 256

#define CHUNK 60

typedef enum structure_type {
	NONE = 0,
	TREE
} structure_type;

typedef struct grid
{
    bool is_active;

	neigbors neigbors;

	structure_type type;

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

}grid;


grid grids[MAX_WORLD_X][MAX_WORLD_Z][MAX_WORLD_Y] = { {{ 0 }} };

glm::vec3 selected_block(0.f);

void initialize_world(std::vector<unsigned int> block_textures, unsigned int skybox_texture)
{
    initialize_block_resources(block_textures);
    initialize_skybox_resources(skybox_texture);

	const siv::PerlinNoise::seed_type seed = 23456;

	const siv::PerlinNoise perlin{ seed };

	for (int x = 0; x < MAX_WORLD_X; ++x)
	{
		for (int z = 0; z < MAX_WORLD_Z; ++z)
		{
			const double noise = (perlin.octave2D_01((x * 0.01), (z * 0.01), 5, 0.7) * 10);

			for (int y = 0; y < noise; ++y)
			{
				grids[x][z][y].is_active = true;

				if (y == noise - 1 && (rand() % 3 + 1) == 1) 
				{
					std::cout << "initialize_world()::TREE_FRAG: x:" << x << " y: " << y << " z: " << z << std::endl;

					grids[x][z][y].type = TREE;
				}
			}
		}
	}

	for (int x = 0; x < MAX_WORLD_X; ++x)
	{
		for (int z = 0; z < MAX_WORLD_Z; ++z)
		{
			for (int y = 0; y < MAX_WORLD_Y; ++y)
			{
				if (grids[x][z][y].is_active)
				{
					grids[x][z][y].neigbors =
					{
						((y + 1) >= BOUND_Y) ? false : grids[x][z][y + 1].is_active,
						((y - 1) < 0) ? false : grids[x][z][y - 1].is_active,
						((x + 1) >= BOUND_X) ? false : grids[x + 1][z][y].is_active,
						((x - 1) < 0) ? false : grids[x - 1][z][y].is_active,
						((z + 1) >= BOUND_Z) ? false : grids[x][z + 1][y].is_active,
						((z - 1) < 0) ? false : grids[x][z - 1][y].is_active
					};
				}
			}
		}
	}
}

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position, glm::vec3 front)
{
	update_block_shader(view, projection, position);

	float max_z = position.z + CHUNK;
	float min_z = position.z - CHUNK;
	float max_x = position.x + CHUNK;
	float min_x = position.x - CHUNK;

	if (max_x < min_x) std::swap(max_x, min_x);
	if (max_z < min_z) std::swap(max_z, min_z);

	max_x = FCLAMP(max_x, 0, 255);
	min_x = FCLAMP(min_x, 0, 255);
	max_z = FCLAMP(max_z, 0, 255);
	min_z = FCLAMP(min_z, 0, 255);

	begin_draw();

	for (size_t x = min_x; x < max_x; ++x)
	{
		for (size_t z = min_z; z < max_z; ++z)
		{
			for (size_t y = 1; y < 15; ++y)
			{
				if (grids[x][z][y].is_active && !grids[x][z][y].is_buried())
				{
					if (is_on_frustum(view, projection, glm::vec3(x,y,z))) 
					{
						draw_block(glm::vec3(x, y, z), grids[x][z][y].neigbors);

						//std::cout << "update_world()::TREE_FRAG: " << grids[x][z][y].type << std::endl;

						if(grids[x][z][y].type == TREE)
						{
							draw_block(glm::vec3(x, y + 1, z), grids[x][z][y+1].neigbors);
							draw_block(glm::vec3(x, y + 2, z), grids[x][z][y+2].neigbors);
							draw_block(glm::vec3(x, y + 3, z), grids[x][z][y+3].neigbors);
							draw_block(glm::vec3(x, y + 4, z), grids[x][z][y + 1].neigbors);
							draw_block(glm::vec3(x, y + 5, z), grids[x][z][y + 2].neigbors);
							draw_block(glm::vec3(x, y + 6, z), grids[x][z][y + 3].neigbors);
							draw_block(glm::vec3(x, y + 7, z), grids[x][z][y + 1].neigbors);
							draw_block(glm::vec3(x, y + 8, z), grids[x][z][y + 2].neigbors);
							draw_block(glm::vec3(x, y + 9, z), grids[x][z][y + 3].neigbors);
						}
					}
				}
			}
		}
	}

	end_draw();

	draw_skybox(glm::mat4(0.f), view, projection, position);
}

bool is_on_frustum(glm::mat4 view, glm::mat4 projection, glm::vec3 position) 
{
	glm::vec4 block_position_clip_space = projection * view * glm::vec4(
			position.x + 0.5f,
			position.y + 0.5f,
			position.z + 0.5f,
			1.0f
	);

	block_position_clip_space /= block_position_clip_space.w;

	float x_ndc = block_position_clip_space.x;
	float y_ndc = block_position_clip_space.y;
	float z_ndc = block_position_clip_space.z;

	if (x_ndc > -1.0f && x_ndc < 1.0f &&
		y_ndc > -1.0f && y_ndc < 1.0f &&
		z_ndc > -1.0f && z_ndc < 1.0f)
	{
		return true;
	}
	return false;

}
