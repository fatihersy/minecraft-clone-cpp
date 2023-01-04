#include "world.h"

#include "block.h"
#include "skybox.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// world space positions of our cubes
#define BOUND_X 50
#define BOUND_Y 5
#define BOUND_Z 50

#define MAX_WORLD_X 50
#define MAX_WORLD_Y 5
#define MAX_WORLD_Z 50

#define CHUNK 50

typedef struct grid
{
    bool is_active;

	neigbors neigbors;
}grid;


grid grids[BOUND_X][BOUND_Y][BOUND_Z] = { 0 };

glm::vec3 selected_block(0.f);

float window_ratio;

void initialize_world(unsigned int block_texture, unsigned int skybox_texture)
{
    initialize_block_resources(block_texture, block_texture, block_texture);
    initialize_skybox_resources(skybox_texture);

	unsigned int grid_count = 0;

	for (int x = 0; x < MAX_WORLD_X; x++)
	{
		for (int z = 0; z < MAX_WORLD_Z; z++)
		{
			for (int y = 0; y < MAX_WORLD_Y; y++)
			{
				grids[x][y][z].is_active = true;
			}
		}
	}

	for (int x = 0; x < MAX_WORLD_X; x++)
	{
		for (int z = 0; z < MAX_WORLD_Z; z++)
		{
			for (int y = 0; y < MAX_WORLD_Y; y++)
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

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position)
{
	update_block_shader(view, projection, position);



	for (int x = 0; x < MAX_WORLD_X; x++)
	{
		for (int z = 0; z < MAX_WORLD_Z; z++)
		{
			for (int y = 0; y < MAX_WORLD_Y; y++)
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

