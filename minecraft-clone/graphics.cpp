#include "graphics.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

//#include "graphic.h"
//#include "shader.h"
//#include "skybox.h"
//#include "camera.h"

unsigned int SCR_WIDTH;
unsigned int SCR_HEIGHT;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static GLFWwindow* window;

int create_window(const char* title, int width, int height, void* update_input, void* mouse_callback, void* mouse_button_callback)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    SCR_WIDTH = width;
    SCR_HEIGHT = height;

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, (GLFWcursorposfun)mouse_callback);
    glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return true;
}

void graphics_update_render()
{

}

void graphics_begin_render()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void graphics_end_render()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}


void terminate_window() { glfwTerminate(); }
void* get_window_instance() { return window; }
unsigned int get_window_width() { return SCR_WIDTH; }
unsigned int get_window_height() { return SCR_HEIGHT; }

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
