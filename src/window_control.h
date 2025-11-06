#pragma once

#include <SDL.h>

#include <chrono>

class Application;

/**
 * Owns the SDL window and renderer plus associated input handling.
 * Provides a simple run loop that delegates application-specific work.
 */
class WindowControl
{
public:
    WindowControl();
    ~WindowControl();

    WindowControl(const WindowControl&) = delete;
    WindowControl& operator=(const WindowControl&) = delete;

    /**
     * Executes the application loop until a quit event is received.
     * @param app Application instance that receives update and render callbacks.
     */
    void run(Application& app);

    /**
     * Signals the loop to exit after the current iteration.
     */
    void quit();

    /**
     * @return Raw pointer to the underlying SDL renderer.
     */
    SDL_Renderer* renderer() const { return renderer_; }

    /**
     * @return Raw pointer to the underlying SDL window.
     */
    SDL_Window* window() const { return window_; }

    /**
     * @return Rectangle describing the content area inside the custom chrome.
     */
    SDL_Rect contentRect() const { return contentRect_; }

private:
    void pumpEvents();
    void handleMouseButtonDown(const SDL_MouseButtonEvent& event);
    void handleMouseButtonUp(const SDL_MouseButtonEvent& event);
    void handleWindowEvent(const SDL_WindowEvent& event);
    void updateDragging();
    float throttleFrameRate(std::chrono::steady_clock::time_point& lastFrame);
    void render(Application& app);

    SDL_Rect makeCloseButtonRect(int windowWidth) const;
    bool pointInRect(int x, int y, const SDL_Rect& rect) const;

    SDL_Window* window_{};
    SDL_Renderer* renderer_{};
    bool dragging_ = false;
    int dragOffsetX_ = 0;
    int dragOffsetY_ = 0;
    bool closeButtonPressed_ = false;
    bool running_ = true;
    SDL_Rect contentRect_{};
};

