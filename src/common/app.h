#pragma once

#include <SDL2/SDL.h>

/**
 * @brief Base root of the app
 */
class App {
public:
    App();
    ~App();

    void beginFrame() const;
    void endFrame() const;

    bool isRunning() const;
    void exit();

private:
    void initSDL();
    void initImgui() const;

private:
    SDL_Window* m_window;
    SDL_GLContext m_glContext;
    static bool m_instanciated;
    bool m_running;
};
