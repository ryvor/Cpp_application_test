#pragma once

#include <SDL.h>

#include <string_view>

/**
 * Renders small 5x7 bitmap text onto an SDL renderer.
 * Glyphs are uppercase-only; characters outside the set are ignored.
 */
void DrawText(SDL_Renderer* renderer, std::string_view text, int x, int y, SDL_Color color);

