#include "application.h"
#include "window_control.h"

#include <cstdio>
#include <exception>

int main(int argc, char* argv[])
{
    (void)argc;
    (void)argv;

    try
    {
        WindowControl window;
        Application app;
        window.run(app);
        return 0;
    }
    catch (const std::exception& ex)
    {
        std::fprintf(stderr, "Application error: %s\n", ex.what());
        return 1;
    }
}
