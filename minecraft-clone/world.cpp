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

#define CHUNK 40

typedef struct grid
{
    bool is_active;

	neigbors neigbors;

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


grid grids[BOUND_X][BOUND_Y][BOUND_Z] = { 0 };

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
			const int noise = (perlin.octave2D_01((x * 0.01), (z * 0.01), 5, 0.7) * 10);

			for (int y = 0; y < noise; ++y)
			{
				grids[x][y][z].is_active = true;
			}
		}
	}

	for (int x = 0; x < MAX_WORLD_X; ++x)
	{
		for (int z = 0; z < MAX_WORLD_Z; ++z)
		{
			for (int y = 0; y < MAX_WORLD_Y; ++y)
			{
				if (grids[x][y][z].is_active)
				{
					grids[x][y][z].neigbors =
					{
						((y + 1) >= BOUND_Y) ? false : grids[x][y + 1][z].is_active,
						((y - 1) < 0) ? false : grids[x][y - 1][z].is_active,
						((x + 1) >= BOUND_X) ? false : grids[x + 1][y][z].is_active,
						((x - 1) < 0) ? false : grids[x - 1][y][z].is_active,
						((z + 1) >= BOUND_Z) ? false : grids[x][y][z - 1].is_active,
						((z - 1) < 0) ? false : grids[x][y][z + 1].is_active
					};
				}
			}
		}
	}
}

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position, glm::vec3 front)
{
	update_block_shader(view, projection, position);

	int max_z = position.z + CHUNK;
	int min_z = position.z - CHUNK;
	int max_x = position.x + CHUNK;
	int min_x = position.x - CHUNK;

	if (max_x < min_x) std::swap(max_x, min_x);
	if (max_z < min_z) std::swap(max_z, min_z);

	max_x = std::clamp(max_x, 0, 256);
	min_x = std::clamp(min_x, 0, 256);
	max_z = std::clamp(max_z, 0, 256);
	min_z = std::clamp(min_z, 0, 256);

	begin_draw();

	for (int x = min_x; x < max_x; ++x)
	{
		for (int z = min_z; z < max_z; ++z)
		{
			for (int y = 1; y < 10; ++y)
			{
				if (grids[x][y][z].is_active && !grids[x][y][z].is_buried())
				{
					if (is_on_frustum(view, projection, glm::vec3(x,y,z))) 
					{
						draw_block(glm::vec3(x, y, z), grids[x][y][z].neigbors);
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
