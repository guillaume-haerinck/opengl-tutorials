#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>

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
        GLCall(glBindVertexArray(vao));
        GLCall(glDrawArrays(GL_TRIANGLES, 0, 3));

        app.endFrame();
    }
    
    return 0;
}
