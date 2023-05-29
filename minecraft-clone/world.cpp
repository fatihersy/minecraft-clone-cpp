#include "world.h"

#include "block.h"
#include "skybox.h"
#include "fdefines.h"
#include "PerlinNoise.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include <iostream>
#include <map>

// world space positions of our cubes
#define BOUND_X 256
#define BOUND_Y 20
#define BOUND_Z 256

#define MAX_WORLD_X 256
#define MAX_WORLD_Y 16
#define MAX_WORLD_Z 256

#define CHUNK 60

typedef struct coord 
{
	coord() : x{ 0 }, z{ 0 }, y{ 0 } {};

	coord(uint16_t _x, uint16_t _z, uint16_t _y) : x{_x}, z{ _z }, y{ _y } {}

	uint16_t x;
	uint16_t z;
	uint16_t y;
};

block matrix[MAX_WORLD_X][MAX_WORLD_Z][MAX_WORLD_Y] = {};
std::vector<coord> tree_coords({});

void initialize_world(std::vector<uint16_t> block_textures, uint16_t skybox_texture)
{
    initialize_block_resources(block_textures);
    initialize_skybox_resources(skybox_texture);

	const siv::PerlinNoise::seed_type seed = 23456;

	const siv::PerlinNoise perlin{ seed };

	int16_t delay_row = 0;
	int16_t delay_col = 0;

	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist6(1, 100);

	for (int x = 0; x < MAX_WORLD_X; ++x)
	{
		for (int z = 0; z < MAX_WORLD_Z; ++z)
		{
			const short noise = (perlin.octave2D_01((x * 0.01), (z * 0.01), 5, 0.7) * 10);

			for (int y = 0; y < noise; ++y)
			{
				matrix[x][z][y].is_active = true;

				if (y == noise - 1 && (dist6(rng) == 1) && (delay_row <= 0 || delay_col <= 0 ) )
				{
					bool is_offset = false;

					for (coord fol2 : tree_coords)
					{
						if ((x - 5 > fol2.x) || (x + 5 < fol2.x)) continue;
						if ((z - 5 > fol2.z) || (z + 5 < fol2.z)) continue;
						if (y != fol2.y) continue;

						is_offset = true;
						break;
					}

					if (!is_offset) 
					{
						matrix[x][z][y].type = TREE;
						tree_coords.push_back(coord(x, z, y));

						if (delay_col <= 0) delay_col = TREE * 6;
						if (delay_row <= 0) delay_row = TREE * 6;
					}
				}

			}

			delay_col--;
		}

		delay_row--;
	}

	for (int x = 0; x < MAX_WORLD_X; ++x)
	{
		for (int z = 0; z < MAX_WORLD_Z; ++z)
		{
			for (int y = 0; y < MAX_WORLD_Y; ++y)
			{
				if (matrix[x][z][y].is_active)
				{
					matrix[x][z][y].neigbors =
					{
						((y + 1) >= BOUND_Y) ? false : matrix[x][z][y + 1].is_active,
						((y - 1) < 0) ? false : matrix[x][z][y - 1].is_active,
						((x + 1) >= BOUND_X) ? false : matrix[x + 1][z][y].is_active,
						((x - 1) < 0) ? false : matrix[x - 1][z][y].is_active,
						((z + 1) >= BOUND_Z) ? false : matrix[x][z + 1][y].is_active,
						((z - 1) < 0) ? false : matrix[x][z - 1][y].is_active
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
				if (matrix[x][z][y].is_active && !matrix[x][z][y].is_buried())
				{
					if (is_on_frustum(view, projection, glm::vec3(x,y,z))) 
					{
						draw_block(glm::vec3(x, y, z), matrix[x][z][y].neigbors);

						//std::cout << "update_world()::TREE_FRAG: " << grids[x][z][y].type << std::endl;

						if(matrix[x][z][y].type == TREE)
						{
							draw_block(glm::vec3(x, y + 1, z), matrix[x][z][y+1].neigbors);
							draw_block(glm::vec3(x, y + 2, z), matrix[x][z][y+2].neigbors);
							draw_block(glm::vec3(x, y + 3, z), matrix[x][z][y+3].neigbors);
							draw_block(glm::vec3(x, y + 4, z), matrix[x][z][y + 1].neigbors);
							draw_block(glm::vec3(x, y + 5, z), matrix[x][z][y + 2].neigbors);
							draw_block(glm::vec3(x, y + 6, z), matrix[x][z][y + 3].neigbors);
							draw_block(glm::vec3(x, y + 7, z), matrix[x][z][y + 1].neigbors);
							draw_block(glm::vec3(x, y + 8, z), matrix[x][z][y + 2].neigbors);
							draw_block(glm::vec3(x, y + 9, z), matrix[x][z][y + 3].neigbors);
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
