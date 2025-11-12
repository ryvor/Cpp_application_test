#include "window_control.h"

#include "app_config.h"
#include "application.h"
#include "text_renderer.h"

#include <stdexcept>
#include <string>

namespace {
    constexpr int kInitialWindowWidth = 960;
    constexpr int kInitialWindowHeight = 600;
    constexpr int kTitleBarHeight = 48;
    constexpr int kCloseButtonSize = 32;
    constexpr int kTitlePadding = 24;
    constexpr int kContentPadding = 12;
    constexpr float kTargetFrameMilliseconds = 16.0f;
}

WindowControl::WindowControl() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw std::runtime_error(std::string{"SDL_Init failed: "} + SDL_GetError());
    }

    window_ = SDL_CreateWindow(
        std::string(AppConfig::kAppName).c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kInitialWindowWidth,
        kInitialWindowHeight,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE);

    if (!window_) {
        SDL_Quit();
        throw std::runtime_error(std::string{"SDL_CreateWindow failed: "} + SDL_GetError());
    }

    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer_) {
        SDL_DestroyWindow(window_);
        SDL_Quit();
        throw std::runtime_error(std::string{"SDL_CreateRenderer failed: "} + SDL_GetError());
    }

    SDL_SetRenderDrawBlendMode(renderer_, SDL_BLENDMODE_BLEND);
}

WindowControl::~WindowControl() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }
    SDL_Quit();
}

void WindowControl::run(Application& app) {
    running_ = true;
    auto lastFrame = std::chrono::steady_clock::now();

    while (running_) {
        pumpEvents();
        const float deltaMs = throttleFrameRate(lastFrame);
        updateDragging();
        app.update(*this, deltaMs);
        render(app);
    }
}

void WindowControl::quit() {
    running_ = false;
}

void WindowControl::pumpEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                quit();
                break;
            case SDL_MOUSEBUTTONDOWN:
                handleMouseButtonDown(event.button);
                break;
            case SDL_MOUSEBUTTONUP:
                handleMouseButtonUp(event.button);
                break;
            case SDL_WINDOWEVENT:
                handleWindowEvent(event.window);
                break;
            default:
                break;
        }
    }
}

void WindowControl::handleMouseButtonDown(const SDL_MouseButtonEvent& event) {
    if (event.button != SDL_BUTTON_LEFT) {
        return;
    }

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window_, &windowWidth, &windowHeight);

    const SDL_Rect closeButtonRect = makeCloseButtonRect(windowWidth);
    if (pointInRect(event.x, event.y, closeButtonRect)) {
        closeButtonPressed_ = true;
        return;
    }

    if (event.y > kTitleBarHeight) {
        return;
    }

    int windowX = 0;
    int windowY = 0;
    SDL_GetWindowPosition(window_, &windowX, &windowY);

    int globalMouseX = 0;
    int globalMouseY = 0;
    SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);

    dragging_ = true;
    dragOffsetX_ = globalMouseX - windowX;
    dragOffsetY_ = globalMouseY - windowY;
    SDL_CaptureMouse(SDL_TRUE);
}

void WindowControl::handleMouseButtonUp(const SDL_MouseButtonEvent& event) {
    if (event.button != SDL_BUTTON_LEFT) {
        return;
    }

    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window_, &windowWidth, &windowHeight);

    const SDL_Rect closeButtonRect = makeCloseButtonRect(windowWidth);
    const bool withinClose = pointInRect(event.x, event.y, closeButtonRect);

    if (closeButtonPressed_ && withinClose) {
        quit();
    }

    closeButtonPressed_ = false;
    dragging_ = false;
    SDL_CaptureMouse(SDL_FALSE);
}

void WindowControl::handleWindowEvent(const SDL_WindowEvent& event) {
    if (event.event == SDL_WINDOWEVENT_CLOSE) {
        quit();
    }
}

void WindowControl::updateDragging() {
    if (!dragging_) {
        return;
    }

    int globalMouseX = 0;
    int globalMouseY = 0;
    SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);
    SDL_SetWindowPosition(window_, globalMouseX - dragOffsetX_, globalMouseY - dragOffsetY_);
}

float WindowControl::throttleFrameRate(std::chrono::steady_clock::time_point& lastFrame) {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration<float, std::milli>(now - lastFrame);

    if (elapsed.count() < kTargetFrameMilliseconds) {
        SDL_Delay(static_cast<Uint32>(kTargetFrameMilliseconds - elapsed.count()));
        now = std::chrono::steady_clock::now();
        elapsed = std::chrono::duration<float, std::milli>(now - lastFrame);
    }

    lastFrame = now;
    return elapsed.count();
}

void WindowControl::render(Application& app) {
    int windowWidth = 0;
    int windowHeight = 0;
    SDL_GetWindowSize(window_, &windowWidth, &windowHeight);

    SDL_SetRenderDrawColor(renderer_, 24, 26, 27, 255);
    SDL_RenderClear(renderer_);

    SDL_Rect titleBarRect{0, 0, windowWidth, kTitleBarHeight};
    SDL_SetRenderDrawColor(renderer_, 46, 52, 64, 255);
    SDL_RenderFillRect(renderer_, &titleBarRect);

    const SDL_Rect closeButtonRect = makeCloseButtonRect(windowWidth);
    const SDL_Color closeButtonColor = closeButtonPressed_ ? SDL_Color{191, 97, 106, 255} : SDL_Color{208, 135, 112, 255};
    SDL_SetRenderDrawColor(renderer_, closeButtonColor.r, closeButtonColor.g, closeButtonColor.b, closeButtonColor.a);
    SDL_RenderFillRect(renderer_, &closeButtonRect);

    SDL_SetRenderDrawColor(renderer_, 46, 52, 64, 255);
    SDL_RenderDrawRect(renderer_, &closeButtonRect);

    const std::string titleBarText = std::string(AppConfig::kDefaultTitleBarText);
    DrawText(renderer_, titleBarText, kTitlePadding, 14, SDL_Color{236, 239, 244, 255});

    SDL_SetRenderDrawColor(renderer_, 236, 239, 244, 255);
    SDL_RenderDrawLine(renderer_, closeButtonRect.x + 8, closeButtonRect.y + 8, closeButtonRect.x + closeButtonRect.w - 8, closeButtonRect.y + closeButtonRect.h - 8);
    SDL_RenderDrawLine(renderer_, closeButtonRect.x + closeButtonRect.w - 8, closeButtonRect.y + 8, closeButtonRect.x + 8, closeButtonRect.y + closeButtonRect.h - 8);

    contentRect_ = SDL_Rect {
        kContentPadding,
        kTitleBarHeight + kContentPadding,
        windowWidth - (kContentPadding * 2),
        windowHeight - kTitleBarHeight - (kContentPadding * 2)};

    SDL_SetRenderDrawColor(renderer_, 67, 76, 94, 255);
    SDL_RenderFillRect(renderer_, &contentRect_);

    app.render(*this);

    SDL_RenderPresent(renderer_);
}

SDL_Rect WindowControl::makeCloseButtonRect(int windowWidth) const {
    return SDL_Rect {
        windowWidth - kCloseButtonSize - kTitlePadding / 2,
        (kTitleBarHeight - kCloseButtonSize) / 2,
        kCloseButtonSize,
        kCloseButtonSize};
}

bool WindowControl::pointInRect(int x, int y, const SDL_Rect& rect) const {
    return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
}
