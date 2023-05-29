#include "block.h"

#include "shader.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

float vertices_quad[] = {
	// positions        // texture coords
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f  // top left 
};
unsigned int indices_quad[] = {  // note that we start from 0!
	0, 1, 3,  // first Triangle
	1, 2, 3   // second Triangle
};


shader_program program;
GLuint block_vao, block_vbo, block_ebo;
std::vector<uint16_t> block_textures;
static glm::mat4 model(1.f);

void initialize_block_resources(std::vector<uint16_t> _block_textures)
{
	program = create_program("D:/Workspace/CLang/Resources/cubemaps.vs", "D:/Workspace/CLang/Resources/cubemaps.fs");

	glGenVertexArrays(1, &block_vao);
	glGenBuffers(1, &block_vbo);
	glGenBuffers(1, &block_ebo);

	glBindVertexArray(block_vao);

	glBindBuffer(GL_ARRAY_BUFFER, block_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_quad), &vertices_quad, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, block_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices_quad), &indices_quad, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    block_textures = _block_textures;

	program.use();
	program.setInt("texture1", 1);
	program.setInt("texture2", 2);
	program.setInt("texture3", 3);

    for (size_t i = 1; i <= block_textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, i);
    }
}

void update_block_shader(glm::mat4 view, glm::mat4 projection, glm::vec3 position)
{
	program.update_program(view, projection, position);
}

void draw_block(glm::vec3 position, neigbors neigbors)
{

    if (!neigbors.up)
    {
        program.setInt("texture_index", 1);
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(position.x, position.y + 0.5, position.z ));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(90.0f, 0.0f, 0.0f));
        program.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    if (!neigbors.down)
    {
        program.setInt("texture_index", 4);
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(position.x, position.y - 0.5, position.z ));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(90.0f, 0.0f, 0.0f));
        program.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    if (!neigbors.right)
    {
        program.setInt("texture_index", 2);
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(position.x + 0.5, position.y, position.z ));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 90.f, 0.f));
        program.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    if (!neigbors.left)
    {
        program.setInt("texture_index", 2);
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(position.x - 0.5, position.y, position.z ));
        model = glm::rotate(model, glm::radians(90.f), glm::vec3(0.0f, 90.f, 0.f));
        program.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    if (!neigbors.front)
    {
        program.setInt("texture_index", 2);
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(position.x, position.y, position.z - 0.5 ));
        model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.0f, 0.0f, 90.0f));
        program.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }


    if (!neigbors.back)
    {
        program.setInt("texture_index", 2);
        model = glm::mat4(1.f);
        model = glm::translate(model, glm::vec3(position.x, position.y, position.z + 0.5 ));
        model = glm::rotate(model, glm::radians(0.f), glm::vec3(0.0f, 0.0f, 90.0f));
        program.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}

void begin_draw() 
{
    glBindVertexArray(block_vao);
}

void end_draw() 
{
    glBindVertexArray(0);
}