#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "common/app.h"
#include "common/gl-exception.h"

#include "square-vao.hpp"

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

	SquareVao squareVAO;

    // ------------------ Vertex shader
    unsigned int vs;
    int success;
    char infoLog[512];
    {
		const char* vsSource = R"(#version 330 core
            layout (location = 0) in vec2 aPos;
            layout (location = 1) in vec3 aCol;
			out vec3 vCol;

            void main() {
                gl_Position = vec4(aPos, 0.0, 1.0) ;
				vCol = aCol;
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
			in vec3 vCol;
            out vec4 FragColor;

            void main() {
                FragColor = vec4(vCol,1.0f);
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

    while (app.isRunning()) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT: app.exit();

            default: break;
            };
        }

        app.beginFrame();

        // Draw call
		GLCall(glUseProgram(pipeline));
		squareVAO.draw();

        app.endFrame();
    }
    
    return 0;
}
