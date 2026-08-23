#pragma once

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "./../manager/window_manager.h"
#include "../utils/geometry.h"

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

    void attachMesh(size_t entityId, Geometry::Mesh *mesh);

    void removeMesh(size_t entityId);
};

extern "C"
{
    void render_system_attach_mesh(size_t entityId, Geometry::Mesh *mesh);

    void render_system_remove_mesh(size_t entityId);
}