#include <SDL2/SDL.h>
#include <glad/glad.h>

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
    {
        const char* vsSource = R"(
            #version 330 core
            layout (location = 0) in vec3 aPos;

            void main() {
                gl_Position = vec4(aPos.x, aPos.y, 1.0)
            }
        )";
        
        vs = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vs, 1, &vsSource, NULL);
        glCompileShader(vs);
    }

    // ------------------ Fragment shader
    unsigned int fs;
    {
        const char* fsSource = R"(
            #version 330 core
            out vec4 FragColor;

            void main() {
                fragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            } 
        )";
        
        fs = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fs, 1, &fsSource, NULL);
        glCompileShader(fs);
    }

    // ------------------ Pipeline
    unsigned int pipeline;
    {
        pipeline = glCreateProgram();
        glAttachShader(pipeline, vs);
        glAttachShader(pipeline, fs);
        glLinkProgram(pipeline);

        // Delete useless data
        glDeleteShader(vs);
        glDeleteShader(fs);
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
        glUseProgram(pipeline);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        app.endFrame();
    }
    
    return 0;
}
