#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "./../manager/window_manager.h"

class RenderSystem : public WindowManager::WindowEventListener
{
public:
    bool init(WindowManager *windowSystem);
    bool render();
    bool shutdown();

    // WindowEventListener
    void onQuit() override;
    void onResize(int width, int height) override;

private:
    struct RenderComponent
    {
        size_t entity_id;
        bgfx::VertexBufferHandle vbh; // Vertex Buffer
        bgfx::IndexBufferHandle ibh;  // Index Buffer
        bgfx::ProgramHandle program;  // Material
    };

    std::vector<RenderComponent> renderComponents;

    int width,
        height;
};

#endif