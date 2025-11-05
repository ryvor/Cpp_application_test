# Custom Title Bar C++ Application

This project provides a minimal, cross-platform SDL2 starter application that opens a borderless window with a custom-drawn title bar. It can be used as a foundation for future applications that require complete control over window chrome and layout.

## Features

- Uses SDL2 to remain portable across Windows, macOS, and Linux.
- Creates a borderless window and renders a bespoke title bar with a close button.
- Implements window dragging so the custom title bar behaves like native chrome.
- Draws lightweight bitmap text without additional font dependencies, keeping the starter template simple.
- Organised with CMake to make integration into larger projects straightforward.

## Building

The project relies on SDL2, which is fetched and built automatically using CMake's `FetchContent`. Ensure you have a C++20 capable compiler and CMake 3.20 or newer.

```bash
cmake -S . -B build
cmake --build build
```

The resulting executable will be located at `build/custom_title_app` (or `build\\Debug\\custom_title_app.exe` on Windows).

## Running

After building, run the executable directly:

```bash
./build/custom_title_app
```

You should see a window with a custom title bar. Drag the window by the title bar area or close it using the custom close button.

## Next steps

- Replace the placeholder bitmap font with your preferred text rendering solution (e.g., SDL_ttf, Dear ImGui, Skia).
- Add additional window controls such as minimise/maximise buttons.
- Integrate application-specific content inside the provided content area.
