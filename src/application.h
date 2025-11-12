#pragma once

#include <SDL.h>

class WindowControl;

/**
 * Example application logic layered on top of the reusable window control.
 * Modify this file and the corresponding source to build new functionality.
 */
class Application
{
public:
    /**
     * Receives per-frame update notifications.
     * @param window Window wrapper exposing SDL handles and helpers.
     * @param deltaMs Time elapsed since the previous frame in milliseconds.
     */
    void update(WindowControl& window, float deltaMs);

    /**
     * Performs all rendering work for the frame.
     * @param window Window wrapper exposing SDL handles and helpers.
     */
    void render(WindowControl& window);
};

