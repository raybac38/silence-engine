#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL3/SDL.h>

#include "system/transform_system.h"
#include <cstdio>

int main()
{

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window =
        SDL_CreateWindow(
            "Silence",
            1280,
            720,
            SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        printf("SDL error: %s\n", SDL_GetError());
        return 1;
    }

    bgfx::PlatformData pd{};

    /// WAYLAND
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    pd.ndt = SDL_GetPointerProperty(
        props,
        SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER,
        nullptr);

    pd.nwh = SDL_GetPointerProperty(
        props,
        SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER,
        nullptr);

    /* X11
SDL_PropertiesID props = SDL_GetWindowProperties(window);

pd.ndt = SDL_GetPointerProperty(
    props,
    SDL_PROP_WINDOW_X11_DISPLAY_POINTER,
    nullptr
);

pd.nwh = (void*)SDL_GetNumberProperty(
    props,
    SDL_PROP_WINDOW_X11_WINDOW_NUMBER,
    0
);

    */

    printf("ndt=%p\n", pd.ndt);
    printf("nwh=%p\n", pd.nwh);

    bgfx::setPlatformData(pd);

    bgfx::Init init{};
    init.type = bgfx::RendererType::Count;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;

    TransformSystem ts = TransformSystem();

    if (!bgfx::init(init))
    {
        return 1; // Initialization failed
    }

    // Insert your main loop here

    bgfx::shutdown();
    return 0;
}
