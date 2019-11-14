#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>

#include "common/app.h"
#include "common/gl-exception.h"
#include "common/square-data.h"

#include "ShaderPipeline.hpp"

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

    // ------------------ Vertex Buffer 1
    unsigned int posVB;
    {
        GLCall(glGenBuffers(1, &posVB));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVB));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(squareData::positions), squareData::positions, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
    }
    
    // ------------------ Vertex Array
    unsigned int vao;
    {
        GLCall(glGenVertexArrays(1, &vao));
        GLCall(glBindVertexArray(vao));

        // Vertex input description
        {
            GLCall(glEnableVertexAttribArray(0));
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, posVB));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));
        }
        
        GLCall(glBindVertexArray(0));
    }

    // ------------------ Index buffer
    unsigned int ib;
    {
        GLCall(glGenBuffers(1, &ib));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareData::indices), squareData::indices, GL_STATIC_DRAW));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    }

	// ------------------ Shader pipeline

	ShaderPipeline shaderPipeline("cheat/res/shader.vert", "cheat/res/shader.frag");

    float counter = 0.0f;
    while (app.isRunning()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: app.exit();

            default: break;
            };
        }

        counter += 0.05f;
        if (counter > 100) {
            counter = 0;
        }

        app.beginFrame();

        // Update uniforms
		shaderPipeline.bind();
        {
            glm::mat4x4 modelMat = glm::rotate(glm::mat4(1.0f), counter, glm::vec3(0, 1, 0));
			shaderPipeline.setUniformMat4f("uModel", modelMat);
        }
        {
            glm::mat4x4 viewMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -10.0f));
            glm::mat4x4 projMat = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 100.0f);
            glm::mat4x4 viewProjMat = projMat * viewMat;
			shaderPipeline.setUniformMat4f("uViewProj", viewProjMat);
        }

        // Draw call
        GLCall(glBindVertexArray(vao));
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib));
        GLCall(glDrawElements(GL_TRIANGLES, std::size(squareData::indices), GL_UNSIGNED_SHORT, (void*) 0));

        app.endFrame();
    }
    
    return 0;
}
