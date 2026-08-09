#include "core.h"
#include "manager/window_manager.h"
#include "system/render_system.h"
#include "system/transform_system.h"
#include "manager/entity_manager.h"
#include "system/script_system.h"
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
    WindowManager window = WindowManager();

    if (!window.init(windowTitle, windowDefaultWidth, windowDefaultHeight))
        exit(EXIT_FAILURE);

    window.addListener(this);

    TransformSystem transform = TransformSystem();
    RenderSystem renderer = RenderSystem();
    if (!renderer.init(&window))
        exit(EXIT_FAILURE);

    /*
        standard loop
        input => gameplay => physic => render
    */
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
void Core::onResize(int width, int height)
{
    printf("NYP\n");
}
