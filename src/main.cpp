#include <SDL.h>

#include <array>
#include <chrono>
#include <cstdint>
#include <cctype>
#include <stdexcept>
#include <string>
#include <string_view>

namespace
{
    constexpr int kInitialWindowWidth = 960;
    constexpr int kInitialWindowHeight = 600;
    constexpr int kTitleBarHeight = 48;
    constexpr int kCloseButtonSize = 32;

    struct Glyph
    {
        std::array<std::uint8_t, 7> rows{};
    };

    struct GlyphEntry
    {
        char character;
        Glyph glyph;
    };

    constexpr std::array<GlyphEntry, 39> kFont{
        GlyphEntry{'A', {{0b01110, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001}}},
        GlyphEntry{'B', {{0b11110, 0b10001, 0b11110, 0b10001, 0b10001, 0b10001, 0b11110}}},
        GlyphEntry{'C', {{0b01110, 0b10001, 0b10000, 0b10000, 0b10000, 0b10001, 0b01110}}},
        GlyphEntry{'D', {{0b11100, 0b10010, 0b10001, 0b10001, 0b10001, 0b10010, 0b11100}}},
        GlyphEntry{'E', {{0b11111, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000, 0b11111}}},
        GlyphEntry{'F', {{0b11111, 0b10000, 0b11110, 0b10000, 0b10000, 0b10000, 0b10000}}},
        GlyphEntry{'G', {{0b01110, 0b10001, 0b10000, 0b10111, 0b10001, 0b10001, 0b01110}}},
        GlyphEntry{'H', {{0b10001, 0b10001, 0b10001, 0b11111, 0b10001, 0b10001, 0b10001}}},
        GlyphEntry{'I', {{0b01110, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}}},
        GlyphEntry{'J', {{0b00111, 0b00010, 0b00010, 0b00010, 0b10010, 0b10010, 0b01100}}},
        GlyphEntry{'K', {{0b10001, 0b10010, 0b10100, 0b11000, 0b10100, 0b10010, 0b10001}}},
        GlyphEntry{'L', {{0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b11111}}},
        GlyphEntry{'M', {{0b10001, 0b11011, 0b10101, 0b10001, 0b10001, 0b10001, 0b10001}}},
        GlyphEntry{'N', {{0b10001, 0b11001, 0b10101, 0b10011, 0b10001, 0b10001, 0b10001}}},
        GlyphEntry{'O', {{0b01110, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110}}},
        GlyphEntry{'P', {{0b11110, 0b10001, 0b10001, 0b11110, 0b10000, 0b10000, 0b10000}}},
        GlyphEntry{'Q', {{0b01110, 0b10001, 0b10001, 0b10001, 0b10101, 0b10010, 0b01101}}},
        GlyphEntry{'R', {{0b11110, 0b10001, 0b10001, 0b11110, 0b10100, 0b10010, 0b10001}}},
        GlyphEntry{'S', {{0b01111, 0b10000, 0b10000, 0b01110, 0b00001, 0b00001, 0b11110}}},
        GlyphEntry{'T', {{0b11111, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100, 0b00100}}},
        GlyphEntry{'U', {{0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01110}}},
        GlyphEntry{'V', {{0b10001, 0b10001, 0b10001, 0b10001, 0b10001, 0b01010, 0b00100}}},
        GlyphEntry{'W', {{0b10001, 0b10001, 0b10001, 0b10101, 0b10101, 0b10101, 0b01010}}},
        GlyphEntry{'X', {{0b10001, 0b10001, 0b01010, 0b00100, 0b01010, 0b10001, 0b10001}}},
        GlyphEntry{'Y', {{0b10001, 0b10001, 0b01010, 0b00100, 0b00100, 0b00100, 0b00100}}},
        GlyphEntry{'Z', {{0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b10000, 0b11111}}},
        GlyphEntry{'0', {{0b01110, 0b10001, 0b10011, 0b10101, 0b11001, 0b10001, 0b01110}}},
        GlyphEntry{'1', {{0b00100, 0b01100, 0b00100, 0b00100, 0b00100, 0b00100, 0b01110}}},
        GlyphEntry{'2', {{0b01110, 0b10001, 0b00001, 0b00010, 0b00100, 0b01000, 0b11111}}},
        GlyphEntry{'3', {{0b11110, 0b00001, 0b00001, 0b01110, 0b00001, 0b00001, 0b11110}}},
        GlyphEntry{'4', {{0b00010, 0b00110, 0b01010, 0b10010, 0b11111, 0b00010, 0b00010}}},
        GlyphEntry{'5', {{0b11111, 0b10000, 0b11110, 0b00001, 0b00001, 0b10001, 0b01110}}},
        GlyphEntry{'6', {{0b01110, 0b10000, 0b11110, 0b10001, 0b10001, 0b10001, 0b01110}}},
        GlyphEntry{'7', {{0b11111, 0b00001, 0b00010, 0b00100, 0b01000, 0b01000, 0b01000}}},
        GlyphEntry{'8', {{0b01110, 0b10001, 0b10001, 0b01110, 0b10001, 0b10001, 0b01110}}},
        GlyphEntry{'9', {{0b01110, 0b10001, 0b10001, 0b01111, 0b00001, 0b00001, 0b01110}}},
        GlyphEntry{'-', {{0b00000, 0b00000, 0b00000, 0b01110, 0b00000, 0b00000, 0b00000}}},
        GlyphEntry{' ', {{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}}}
    };

    const Glyph* findGlyph(char ch)
    {
        for (const auto& entry : kFont)
        {
            if (entry.character == ch)
            {
                return &entry.glyph;
            }
        }
        return nullptr;
    }

    void drawGlyph(SDL_Renderer* renderer, const Glyph& glyph, int x, int y, SDL_Color color)
    {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        constexpr int pixelWidth = 2;
        constexpr int pixelHeight = 2;

        for (std::size_t row = 0; row < glyph.rows.size(); ++row)
        {
            for (int col = 0; col < 5; ++col)
            {
                if (glyph.rows[row] & (1 << (4 - col)))
                {
                    SDL_Rect rect{ x + col * pixelWidth, y + static_cast<int>(row) * pixelHeight, pixelWidth, pixelHeight };
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }

    void drawText(SDL_Renderer* renderer, std::string_view text, int x, int y, SDL_Color color)
    {
        int penX = x;
        for (char ch : text)
        {
            char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
            if (const Glyph* glyph = findGlyph(upper))
            {
                drawGlyph(renderer, *glyph, penX, y, color);
            }
            penX += 12;
        }
    }

    bool pointInRect(int x, int y, const SDL_Rect& rect)
    {
        return x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h;
    }

    struct WindowState
    {
        SDL_Window* window{};
        SDL_Renderer* renderer{};
        bool dragging = false;
        int dragOffsetX = 0;
        int dragOffsetY = 0;
        bool closeButtonPressed = false;
    };

    WindowState createWindow()
    {
        if (SDL_Init(SDL_INIT_VIDEO) != 0)
        {
            throw std::runtime_error(std::string{"SDL_Init failed: "} + SDL_GetError());
        }

        SDL_Window* window = SDL_CreateWindow(
            "Custom Title App",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            kInitialWindowWidth,
            kInitialWindowHeight,
            SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE
        );

        if (!window)
        {
            SDL_Quit();
            throw std::runtime_error(std::string{"SDL_CreateWindow failed: "} + SDL_GetError());
        }

        SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (!renderer)
        {
            SDL_DestroyWindow(window);
            SDL_Quit();
            throw std::runtime_error(std::string{"SDL_CreateRenderer failed: "} + SDL_GetError());
        }

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        return { window, renderer };
    }

    void destroyWindow(WindowState& state)
    {
        if (state.renderer)
        {
            SDL_DestroyRenderer(state.renderer);
            state.renderer = nullptr;
        }
        if (state.window)
        {
            SDL_DestroyWindow(state.window);
            state.window = nullptr;
        }
        SDL_Quit();
    }

    SDL_Rect makeCloseButtonRect(int windowWidth)
    {
        return SDL_Rect{ windowWidth - kCloseButtonSize - 12, (kTitleBarHeight - kCloseButtonSize) / 2, kCloseButtonSize, kCloseButtonSize };
    }

    void renderWindow(WindowState& state)
    {
        int windowWidth = 0;
        int windowHeight = 0;
        SDL_GetWindowSize(state.window, &windowWidth, &windowHeight);

        SDL_SetRenderDrawColor(state.renderer, 24, 26, 27, 255);
        SDL_RenderClear(state.renderer);

        SDL_Rect titleBarRect{ 0, 0, windowWidth, kTitleBarHeight };
        SDL_SetRenderDrawColor(state.renderer, 46, 52, 64, 255);
        SDL_RenderFillRect(state.renderer, &titleBarRect);

        SDL_Rect closeButtonRect = makeCloseButtonRect(windowWidth);
        const SDL_Color closeButtonColor = state.closeButtonPressed ? SDL_Color{ 191, 97, 106, 255 } : SDL_Color{ 208, 135, 112, 255 };
        SDL_SetRenderDrawColor(state.renderer, closeButtonColor.r, closeButtonColor.g, closeButtonColor.b, closeButtonColor.a);
        SDL_RenderFillRect(state.renderer, &closeButtonRect);

        SDL_SetRenderDrawColor(state.renderer, 46, 52, 64, 255);
        SDL_RenderDrawRect(state.renderer, &closeButtonRect);

        SDL_SetRenderDrawColor(state.renderer, 236, 239, 244, 255);
        SDL_RenderDrawLine(state.renderer, closeButtonRect.x + 8, closeButtonRect.y + 8, closeButtonRect.x + closeButtonRect.w - 8, closeButtonRect.y + closeButtonRect.h - 8);
        SDL_RenderDrawLine(state.renderer, closeButtonRect.x + closeButtonRect.w - 8, closeButtonRect.y + 8, closeButtonRect.x + 8, closeButtonRect.y + closeButtonRect.h - 8);

        drawText(state.renderer, "CUSTOM WINDOW BASE", 24, 14, SDL_Color{ 236, 239, 244, 255 });

        SDL_Rect contentRect{ 12, kTitleBarHeight + 12, windowWidth - 24, windowHeight - kTitleBarHeight - 24 };
        SDL_SetRenderDrawColor(state.renderer, 67, 76, 94, 255);
        SDL_RenderFillRect(state.renderer, &contentRect);

        drawText(state.renderer, "READY FOR YOUR IDEAS", 32, kTitleBarHeight + 40, SDL_Color{ 163, 190, 140, 255 });

        SDL_RenderPresent(state.renderer);
    }
}

int main()
{
    try
    {
        WindowState state = createWindow();

        bool running = true;
        auto lastFrame = std::chrono::steady_clock::now();

        while (running)
        {
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                    case SDL_QUIT:
                        running = false;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                    {
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            int mouseX = event.button.x;
                            int mouseY = event.button.y;
                            int windowWidth = 0;
                            int windowHeight = 0;
                            SDL_GetWindowSize(state.window, &windowWidth, &windowHeight);

                            SDL_Rect closeButtonRect = makeCloseButtonRect(windowWidth);

                            if (pointInRect(mouseX, mouseY, closeButtonRect))
                            {
                                state.closeButtonPressed = true;
                            }
                            else if (mouseY <= kTitleBarHeight)
                            {
                                int windowX = 0;
                                int windowY = 0;
                                SDL_GetWindowPosition(state.window, &windowX, &windowY);

                                int globalMouseX = 0;
                                int globalMouseY = 0;
                                SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);

                                state.dragging = true;
                                state.dragOffsetX = globalMouseX - windowX;
                                state.dragOffsetY = globalMouseY - windowY;
                                SDL_CaptureMouse(SDL_TRUE);
                            }
                        }
                        break;
                    }
                    case SDL_MOUSEBUTTONUP:
                    {
                        if (event.button.button == SDL_BUTTON_LEFT)
                        {
                            int mouseX = event.button.x;
                            int mouseY = event.button.y;
                            int windowWidth = 0;
                            int windowHeight = 0;
                            SDL_GetWindowSize(state.window, &windowWidth, &windowHeight);

                            SDL_Rect closeButtonRect = makeCloseButtonRect(windowWidth);
                            bool withinClose = pointInRect(mouseX, mouseY, closeButtonRect);

                            if (state.closeButtonPressed && withinClose)
                            {
                                running = false;
                            }

                            state.closeButtonPressed = false;
                            state.dragging = false;
                            SDL_CaptureMouse(SDL_FALSE);
                        }
                        break;
                    }
                    case SDL_WINDOWEVENT:
                        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
                        {
                            running = false;
                        }
                        break;
                    default:
                        break;
                }
            }

            auto now = std::chrono::steady_clock::now();
            const auto frameDuration = std::chrono::duration<float, std::milli>(now - lastFrame);
            if (frameDuration.count() < 16.0f)
            {
                SDL_Delay(static_cast<Uint32>(16.0f - frameDuration.count()));
            }
            lastFrame = now;

            if (state.dragging)
            {
                int globalMouseX = 0;
                int globalMouseY = 0;
                SDL_GetGlobalMouseState(&globalMouseX, &globalMouseY);
                SDL_SetWindowPosition(state.window, globalMouseX - state.dragOffsetX, globalMouseY - state.dragOffsetY);
            }

            renderWindow(state);
        }

        destroyWindow(state);
        return 0;
    }
    catch (const std::exception& ex)
    {
        SDL_Log("Application error: %s", ex.what());
        return 1;
    }
}
