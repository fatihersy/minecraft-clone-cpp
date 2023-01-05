#include "world.h"

#include "block.h"
#include "skybox.h"
#include "fdefines.h"
#include "PerlinNoise.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
						((z + 1) >= BOUND_Z) ? false : grids[x][y][z + 1].is_active,
						((z - 1) < 0) ? false : grids[x][y][z - 1].is_active
					};
				}
			}
		}
	}
}

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position, glm::vec3 front)
{
	update_block_shader(view, projection, position);

	const int chunk_x = (front.x < 0) ? -CHUNK : CHUNK;
	const int chunk_z = (front.z < 0) ? -CHUNK : CHUNK;

	const int near_x = FMAX(position.x + -chunk_x, 0);
	const int near_y = FMAX(position.y - 15, 0);
	const int near_z = FMAX(position.z + -chunk_z, 0);

	const int far_x = FMIN(position.x + chunk_x, BOUND_X);
	const int far_y = FMIN(position.y + CHUNK, MAX_WORLD_Y);
	const int far_z = FMIN(position.z + chunk_z, BOUND_Z);

	int max_x = FMAX(far_x, near_x);
	int min_x = FMIN(far_x, near_x);
	int max_z = FMAX(far_z, near_z);
	int min_z = FMIN(far_z, near_z);

	max_x = FCLAMP(max_x, 0, 256);
	min_x = FCLAMP(min_x, 0, 256);
	max_z = FCLAMP(max_z, 0, 256);
	min_z = FCLAMP(min_z, 0, 256);

	for (int x = min_x; x < max_x; ++x)
	{
		for (int z = min_z; z < max_z; ++z)
		{
			for (int y = near_y; y < far_y; ++y)
			{
				if (grids[x][y][z].is_active)
				{
					draw_block(glm::vec3(x,y,z), grids[x][y][z].neigbors);
				}
			}
		}
	}

	draw_skybox(glm::mat4(0.f), view, projection, position);
}

