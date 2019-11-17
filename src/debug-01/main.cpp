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
        
        glGenBuffers(1, &vbo);
        GLCall(glBindBuffer(GL_FRAMEBUFFER, vbo));
        glBufferData(GL_FRAMEBUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_FRAMEBUFFER, 0);
    }
    
    // ------------------ Vertex Array
    unsigned int vao;
    {
        glGenVertexArrays(0, &vao);
        glBindVertexArray(vao);

        // Vertex input description
        {
            glEnableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
        }

        glBindVertexArray(0);
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
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        app.endFrame();
    }
    
    return 0;
}
