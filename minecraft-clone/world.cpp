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
}grid;

grid grids[BOUND_X][BOUND_Y][BOUND_Z] = { 0 };

glm::vec3 selected_block(0.f);

float window_ratio;

void initialize_world(unsigned int skybox_texture) 
{
    initialize_block_resources(skybox_texture);
    initialize_skybox_resources(skybox_texture);

	unsigned int grid_count = 0;

	for (size_t x = 0; x < MAX_WORLD_X; x++)
	{
		for (size_t z = 0; z < MAX_WORLD_Z; z++)
		{
			//int max_y = round_up(perlin2d(x, z, 0.1f, 4.0f) * 10);

			for (size_t y = 0; y < MAX_WORLD_Y; y++)
			{

				grids[x][y][z].is_active = true;

				//grids[x][y][z].block_type = (y + 1 == max_y) ? GRASS : DIRT;
			}
		}
	}

}

void update_world(glm::mat4 view, glm::mat4 projection, glm::vec3 position)
{
	glm::mat4 model = glm::mat4(1.0f);

	for (int x = 0; x < MAX_WORLD_X; x++)
	{
		for (int z = 0; z < MAX_WORLD_Z; z++)
		{
			for (int y = 0; y < MAX_WORLD_Y; y++)
			{
				if (grids[x][y][z].is_active)
				{
					model = glm::mat4(1.0f);
					model = glm::translate(model, glm::vec3(x,y,z));
					update_block_shader(model, view, projection, position);
					draw_block();
				}
			}
		}
	}

	draw_skybox(glm::mat4(0.f), view, projection, position);
}

