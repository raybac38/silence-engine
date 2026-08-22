#pragma once

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "./../manager/window_manager.h"

namespace RenderSystem
{

    /**
     * @breif Init render system
     * @return return true if success
     */
    bool init();

    /**
     * @brief Render the next frame
     */
    void render();

    /**
     * @breif Shutdown render system
     */
    void shutdown();

    /**
     * @breif Resize the view
     * @param width width render size
     * @param height height render size
     */
    void resize(int width, int height);
};
