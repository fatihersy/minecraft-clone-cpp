#include <iostream>

#include "platform.h"
#include "world.h"

int main()
{
    float window_width = 1280;
    float window_height = 720;

    initialize_platform("minecraft-clone", window_width, window_height);

    initialize_camera(window_width, window_height);

    unsigned int skybox_texture = loadCubemap(skybox_faces);
    unsigned int block_texture = loadTexture("D:/Workspace/grass_top.jpg", false);

    initialize_world(block_texture, skybox_texture);

    while (platform_pump_messages())
    {
        platform_begin_frame();

        camera_metrice_packet camera_metrices = camera_update_packet();

        update_world
        (
            camera_metrices.view_matrix,
            camera_metrices.projection_matrix,
            camera_metrices.get_camera_position
        );

        platform_end_frame();
    }

    return 0;
}



