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

namespace Core
{
    namespace
    {
        bool quit = false;
    };

    void run()
    {
        if (!WindowManager::open(windowTitle, windowDefaultWidth, windowDefaultHeight))
            exit(EXIT_FAILURE);

        if (!RenderSystem::init())
            exit(EXIT_FAILURE);

        // init();

        /*
            standard loop
            input => gameplay => physic => render
        */
        while (!quit)
        {
            WindowManager::pollEvent();
            ScriptSystem::update();
            RenderSystem::render();
        }
    }

    void init()
    {
        size_t id = EntityManager::allocateEntityId();
        ScriptSystem::attachScript(id, "src/lua/main.lua");
    }

    void shutdown()
    {
        quit = true;
    }
}
