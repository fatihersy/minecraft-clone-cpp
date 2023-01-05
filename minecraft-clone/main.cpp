#include <iostream>

#include "platform.h"
#include "world.h"

int main()
{
    float window_width = 1280;
    float window_height = 720;

    initialize_platform("minecraft-clone", window_width, window_height);

    initialize_camera(window_width, window_height);

    std::vector<unsigned int> block_textures;

    block_textures.push_back(loadTexture("D:/Workspace/grass_top.jpg", false));
    block_textures.push_back(loadTexture("D:/Workspace/grass_side.jpg", false));
    block_textures.push_back(loadTexture("D:/Workspace/grass_bottom.jpg", false));

    unsigned int skybox_texture = loadCubemap(skybox_faces);
    initialize_world(block_textures, skybox_texture);

    while (platform_pump_messages())
    {
        platform_begin_frame();

        camera_metrice_packet camera_metrices = camera_update_packet();

        update_world
        (
            camera_metrices.view_matrix,
            camera_metrices.projection_matrix,
            camera_metrices.get_camera_position,
            camera_metrices.camera_front
        );

        platform_end_frame();
    }

    return 0;
}



