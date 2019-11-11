#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <spdlog/spdlog.h>
#include <debug_break/debug_break.h>

#include "common/app.h"
#include "common/gl-exception.h"

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

     // ------------------ Vertex Buffer
    unsigned int vbo;
    {
        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f,  0.5f, 0.0f
        };
        
        GLCall(glGenBuffers(1, &vbo));
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW));
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
            GLCall(glBindBuffer(GL_ARRAY_BUFFER, vbo));
            GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL));
        }

        GLCall(glBindVertexArray(0));
    }
    

    // ------------------ Vertex shader
    unsigned int vs;
    int success;
    char infoLog[512];
    {
        const char* vsSource = R"(#version 330 core
            layout (location = 0) in vec3 aPos;

            void main() {
                gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
            }
        )";
        
        vs = glCreateShader(GL_VERTEX_SHADER);
        GLCall(glShaderSource(vs, 1, &vsSource, NULL));
        GLCall(glCompileShader(vs));

        // Check compilation
        glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
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

            void main()
            {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
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
            ImGui_ImplSDL2_ProcessEvent(&e);
            switch (e.type) {
            case SDL_QUIT: app.exit();

            default: break;
            };
        }

        app.beginFrame();

        // Draw call
        GLCall((glUseProgram(pipeline));
        GLCall((glBindVertexArray(vao));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        app.endFrame();
    }
    
    return 0;
}
