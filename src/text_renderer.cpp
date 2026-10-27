#include "text_renderer.h"

#include <array>
#include <cctype>
#include <cstdint>

namespace
{
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
        GlyphEntry{' ', {{0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000}}}};

    const Glyph* findGlyph(char character)
    {
        for (const auto& entry : kFont)
        {
            if (entry.character == character)
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
                    SDL_Rect rect{x + col * pixelWidth, y + static_cast<int>(row) * pixelHeight, pixelWidth, pixelHeight};
                    SDL_RenderFillRect(renderer, &rect);
                }
            }
        }
    }
} // namespace

void DrawText(SDL_Renderer* renderer, std::string_view text, int x, int y, SDL_Color color)
{
    int penX = x;
    for (char ch : text)
    {
        const char upper = static_cast<char>(std::toupper(static_cast<unsigned char>(ch)));
        if (const Glyph* glyph = findGlyph(upper))
        {
            drawGlyph(renderer, *glyph, penX, y, color);
        }
        penX += 12;
    }
}

