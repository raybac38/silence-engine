#include "core.h"
#include "manager/window_manager.h"
#include "system/render_system.h"
#include "system/transform_system.h"
#include "manager/entity_manager.h"
#include "system/input_system.hpp"
#include "system/script_system.h"
#include <string>

#include <chrono>
#include <iostream>

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

        auto lastTime = std::chrono::high_resolution_clock::now();
        int frameCount = 0;
        float fps = 0.0f;

        /*
            standard loop
            input => gameplay => physic => render
        */
        auto lastFrameTime = std::chrono::high_resolution_clock::now();

        while (!quit)
        {
            auto currentTime = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration<float>(currentTime - lastFrameTime).count();
            lastFrameTime = currentTime;

            frameCount++;
            std::chrono::duration<float> elapsedTime = currentTime - lastTime;

            if (elapsedTime.count() >= 1.0f)
            {
                fps = static_cast<float>(frameCount) / elapsedTime.count();

                WindowManager::setTitle(windowTitle + " | FPS: " + std::to_string(static_cast<int>(fps)));

                frameCount = 0;
                lastTime = currentTime;
            }

            WindowManager::pollEvent();
            InputSystem::update();

            ScriptSystem::update(dt);
            RenderSystem::render();
        }
    }

    void init()
    {
        size_t id = EntityManager::allocateEntityId();
        ScriptSystem::attachScript(id, "./assets/scripts/main.lua");
    }

    void shutdown()
    {
        quit = true;
    }
}
