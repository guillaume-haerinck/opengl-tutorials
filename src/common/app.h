#pragma once

#include <SDL2/SDL.h>

/**
 * @brief Base root of the app
 */
class App {
public:
    App();
    ~App();

    /**
     * @brief Prepare for a new frame
     * @note Must be called after handling SDL events
     */
    void beginFrame() const;

    /**
     * @brief Render the preparred frame
     */
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
