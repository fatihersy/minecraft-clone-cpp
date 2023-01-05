#include "platform.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

float delta_time = 0.0f;
float last_frame = 0.0f;
float current_frame = 0.0f;

bool firstMouse = true;

float lastX;
float lastY;

float window_width;
float window_height;

bool polymode_switch = true;

// callbacks
void update_input(void* window, float delta_time);
void mouse_callback(void* window, double xposIn, double yposIn);
void scroll_callback(void* window, double xoffset, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// private functions
int create_window(const char* title, int width, int height);

static GLFWwindow* window;

void initialize_platform(const char* window_title, float _window_width, float _window_height) 
{
    window_width = _window_width;
    window_height = _window_height;

    create_window(window_title, window_width, window_height);

    lastX = window_width / 2.0f;
    lastY = window_height / 2.0f;
}

void platform_begin_frame() 
{
    current_frame = static_cast<float>(glfwGetTime());
    delta_time = current_frame - last_frame;

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    (polymode_switch) 
        ? glPolygonMode(GL_FRONT_AND_BACK, GL_FILL)
        : glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    

}

void platform_end_frame() 
{
    glfwSwapBuffers(window);
    glfwPollEvents();

    update_input(window, delta_time);

    last_frame = current_frame;
}

bool platform_pump_messages() 
{
    return !glfwWindowShouldClose(window);
}

void shutdown_platform()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}


int create_window(const char* title, int width, int height)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_width = width;
    window_height = height;

    window = glfwCreateWindow(window_width, window_height, title, NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)mouse_callback);
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST);

    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return true;
}

void update_input(void* window, float delta_time)
{
    if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose((GLFWwindow*)window, true);

    if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_W) == GLFW_PRESS)
        move_camera(FORWARD, delta_time);
    if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_S) == GLFW_PRESS)
        move_camera(BACKWARD, delta_time);
    if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_A) == GLFW_PRESS)
        move_camera(LEFT, delta_time);
    if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_D) == GLFW_PRESS)
        move_camera(RIGHT, delta_time);
    if (glfwGetKey((GLFWwindow*)window, GLFW_KEY_P) == GLFW_PRESS)
    {
        if (polymode_switch) 
        {
            polymode_switch = false;
            glfwWaitEventsTimeout(0.7f);
        } 
        else 
        {
            polymode_switch = true;
            glfwWaitEventsTimeout(0.7f);
        }
    }
}


void mouse_callback(void* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    rotate_camera(xoffset, yoffset);
}


void scroll_callback(void* window, double xoffset, double yoffset)
{
    //camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(std::string path, bool is_png)
{
    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, is_png ? GL_RGBA : GL_RGB, width, height, 0, is_png ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture\n");
    }
    stbi_image_free(data);

    return texture;
}

// loads a cubemap texture from 6 individual texture faces
// order:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// -------------------------------------------------------
unsigned int loadCubemap(std::vector<std::string> skybox_faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(false); // tell stb_image.h to flip loaded texture's on the y-axis.

    for (unsigned int i = 0; i < skybox_faces.size(); i++)
    {
        unsigned char* data = stbi_load(skybox_faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << skybox_faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

