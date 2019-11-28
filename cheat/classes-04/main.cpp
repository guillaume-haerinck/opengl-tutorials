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
#include "CubeMesh.hpp"

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

	// ------------------ Cube mesh

	CubeMesh cube;
	cube.addCube(glm::vec3(0.));

	// ------------------ Shader pipeline

	ShaderPipeline shaderPipeline("res/cheat-classes04.vert", "res/shader.frag");

    float counter = 0.0f;
    while (app.isRunning()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: app.exit();
				break;
			case SDL_MOUSEBUTTONDOWN:
				int x, y;
				SDL_GetMouseState(&x, &y);
				cube.addCube(glm::vec3((x / 325.0f -1.0f) * 6.0f, -(y / 325.0f -1.0f) * 6.0f, 1.0f));
				break;

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
		cube.draw();

        app.endFrame();
    }
    
    return 0;
}
