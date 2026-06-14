#ifndef RENDER_SYSTEM_H
#define RENDER_SYSTEM_H

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include "./../window_system.h"

class RenderSystem : public WindowSystem::WindowEventListener
{
public:
    bool init(WindowSystem *windowSystem);
    bool render();
    bool shutdown();

    // WindowEventListener
    void onQuit() override;
    void onResize(int width, int height) override;

private:
};

#endif