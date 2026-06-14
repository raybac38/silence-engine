#include "core.h"
#include "window_system.h"
#include "system/render_system.h"
#include <string>

static const std::string windowTitle = "Silence";
static const int windowDefaultHeight = 720;
static const int windowDefaultWidth = 1280;

Core::Core()
{
    quit = false;
}

void Core::run()
{
    WindowSystem window = WindowSystem();

    if (!window.init(windowTitle, windowDefaultWidth, windowDefaultHeight))
        exit(EXIT_FAILURE);

    window.addListener(this);

    RenderSystem renderer = RenderSystem();
    if (!renderer.init(&window))
        exit(EXIT_FAILURE);

    while (!quit)
    {
        window.pollEvent();
        renderer.render();
    }
}

void Core::onQuit()
{
    this->quit = true;
}
void Core::onResize([[maybe_unused]] int width, [[maybe_unused]] int height)
{
}
