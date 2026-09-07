#pragma once

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "./../manager/window_manager.hpp"
#include "./../manager/ressource_manager.hpp"

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

    void attachMesh(size_t entityId, RessourceManager::Manager<Geometry::Mesh>::Handle handle);

    void removeMesh(size_t entityId);

    void setCamera(size_t entityId);
};

extern "C"
{
    void render_system_attach_mesh(size_t entityId, RessourceManager::Manager<Geometry::Mesh>::Handle handle);

    void render_system_remove_mesh(size_t entityId);

    void render_system_set_camera(size_t entityId);
}