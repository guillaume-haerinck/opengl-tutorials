#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>

#include "common/app.h"

int main(int argc, char *argv[]) {
    App app;

    glClearColor(1, 0, 1, 1);

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

        ImGui::Begin("Main debug window");
            ImGui::Text("Hello world !");
            ImGui::Text("Hello world !");
            ImGui::Text("Hello world !");
            ImGui::Text("Hello world !");
            ImGui::Text("Hello world !");
            ImGui::Text("Hello world !");
            ImGui::Text("Hello world !");
        ImGui::End();

        app.endFrame();
    }
    
    return 0;
}
