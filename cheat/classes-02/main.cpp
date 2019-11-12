#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/app.h"
#include "common/gl-exception.h"

#include "imgui.h"

#include "cube-vao.hpp"

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

	CubeVAO cubeVAO;

    // ------------------ Vertex shader
    unsigned int vs;
    int success;
    char infoLog[512];
    {
        const char* vsSource = R"(#version 330 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec3 aNormal;
			out vec3 vLocalPos;
			out vec3 vNormal;
			uniform mat4 uProj;

            void main() {
                gl_Position = uProj * vec4(aPos+vec3(0.,0.,-2.0), 1.0) ;
				vLocalPos = aPos;
				vNormal = aNormal;
            }
        )";
        
        vs = glCreateShader(GL_VERTEX_SHADER);
        GLCall(glShaderSource(vs, 1, &vsSource, NULL));
        GLCall(glCompileShader(vs));

        // Check compilation
        GLCall(glGetShaderiv(vs, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLCall(glGetShaderInfoLog(vs, 512, NULL, infoLog));
            spdlog::critical("[VertexShader] Compilation failed : {}", infoLog);
            debug_break();
        }
    }

    // ------------------ Fragment shader
    unsigned int fs;
    {
		const char* fsSource = R"(#version 330 core
            out vec4 FragColor;
			in vec3 vLocalPos;
			in vec3 vNormal;

            void main() {
                FragColor = vec4(vLocalPos+vec3(0.5),1.0f);
				float t = abs(dot( vNormal, normalize(vec3(0.3,-0.2,0.8))));
				FragColor *= t;
            } 
        )";
        
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        GLCall(glShaderSource(fs, 1, &fsSource, NULL));
        GLCall(glCompileShader(fs));

        // Check compilation
        int success;
        GLCall(glGetShaderiv(fs, GL_COMPILE_STATUS, &success));
        if (!success) {
            GLCall(glGetShaderInfoLog(fs, 512, NULL, infoLog));
            spdlog::critical("[FragmentShader] Compilation failed : {}", infoLog);
            debug_break();
        }
    }

    // ------------------ Pipeline
    unsigned int pipeline;
    {
        pipeline = glCreateProgram();
        GLCall(glAttachShader(pipeline, vs));
        GLCall(glAttachShader(pipeline, fs));
        GLCall(glLinkProgram(pipeline));

        // Check compilation
        GLCall(glGetProgramiv(pipeline, GL_LINK_STATUS, &success));
        if (!success) {
            GLCall(glGetProgramInfoLog(pipeline, 512, NULL, infoLog));
            spdlog::critical("[Pipeline] Link failed : {}", infoLog);
            debug_break();
        }

        // Delete useless data
        GLCall(glDeleteShader(vs));
        GLCall(glDeleteShader(fs));
    }

	// ------------------ Uniforms
	int projMatUniform;
	float fov = 1.0f;
	glm::mat4 projMat;

	GLCall(glUseProgram(pipeline));
	{

		projMatUniform = glGetUniformLocation(pipeline, "uProj");
	}

    while (app.isRunning()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: app.exit();

            default: break;
            };
        }

        app.beginFrame();

		// ImGUI

		ImGui::Begin("Camera");
		ImGui::SliderFloat("FOV", &fov, 0.0f, 3.0f);
		ImGui::End();

        // Draw call
		projMat = glm::perspective(fov, 1.0f, 0.1f, 10.0f);
		GLCall(glUniformMatrix4fv(projMatUniform, 1, GL_FALSE, &projMat[0][0]));

		cubeVAO.draw();

        app.endFrame();
    }
    
    return 0;
}
