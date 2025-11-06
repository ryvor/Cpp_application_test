#include "application.h"

#include "app_config.h"
#include "text_renderer.h"
#include "window_control.h"

#include <string>

void Application::update(WindowControl& window, float deltaMs) {
    (void)window;
    (void)deltaMs;
}

void Application::render(WindowControl& window) {
    SDL_Renderer* renderer = window.renderer();
    const SDL_Rect content = window.contentRect();

    DrawText(renderer, std::string(AppConfig::kAppName) + " READY", content.x + 20, content.y + 16, SDL_Color{236, 239, 244, 255});
}

