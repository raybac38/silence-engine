#include "render_system.h"

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <bgfx/platform.h>
#include "../manager/window_manager.h"
#include <iostream>

namespace RenderSystem
{
    namespace
    {
        struct RenderComponent
        {
            size_t entity_id;
            bgfx::VertexBufferHandle vbh; // Vertex Buffer
            bgfx::IndexBufferHandle ibh;  // Index Buffer
            bgfx::ProgramHandle program;  // Material
        };

        std::vector<RenderComponent> renderComponents;

        int render_width,
            render_height;
    };

    bool init()
    {
        if (!WindowManager::isOpen())
        {
            std::cerr << "[ERROR] RenderSystem : window is not open" << std::endl;
            return false;
        }

        bgfx::PlatformData pd{};

        SDL_PropertiesID props = WindowManager::getProperties();

        const char *video_driver = SDL_GetCurrentVideoDriver();

        // Linux Wayland
        if (SDL_strcmp(video_driver, "wayland") == 0)
        {
            pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
            pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
        }
        // Linux x11
        else if (SDL_strcmp(video_driver, "x11") == 0)
        {
            pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
            // Window ID is an integer, need to cast to a void* for bgfx
            pd.nwh = reinterpret_cast<void *>(SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
        }
        // Windows
        else if (SDL_strcmp(video_driver, "windows") == 0)
        {
            // no display needed for bgfx on Windows
            pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WIN32_HWND_POINTER, nullptr);
        }
        // macOS
        else if (SDL_strcmp(video_driver, "cocoa") == 0)
        {
            pd.ndt = nullptr;
            // no display needed for bgfx on macOS
            pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_COCOA_WINDOW_POINTER, nullptr);
        }
        // Linux without x11 or Wayland
        else if (SDL_strcmp(video_driver, "kmsdrm") == 0)
        {
            pd.ndt = nullptr;
            pd.nwh = reinterpret_cast<void *>(SDL_GetNumberProperty(props, SDL_PROP_WINDOW_KMSDRM_DRM_FD_NUMBER, -1));
        }
        else
        {
            std::cerr << "[ERROR] RenderSystem : driver not handle (" << video_driver << ")" << std::endl;
            return false;
        }

        std::cout << "[INFO] RenderSystem : platform defined (" << video_driver << ")" << std::endl;

        if (!pd.nwh)
        {
            std::cerr << "[ERROR] RenderSystem : cannot get window handler" << std::endl;
            return false;
        }

        bgfx::Init init{};
        init.type = bgfx::RendererType::Count;
        init.platformData = pd;

        init.resolution.width = WindowManager::getWidth();
        init.resolution.height = WindowManager::getHeight();
        init.resolution.reset = BGFX_RESET_VSYNC;

        if (!bgfx::init(init))
        {
            std::cerr << "[ERROR] RenderSystem : failed initiate BGFX" << std::endl;
            return false;
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, WindowManager::getWidth(), WindowManager::getHeight());

        return true;
    }

    void render()
    {
        bgfx::touch(0); // submit empty to the fram, that indirectly clear the window

        const bx::Vec3 at = {0.0f, 1.0f, 0.0f};
        const bx::Vec3 eye = {0.0f, 1.0f, -2.5f};

        {
            float view[16];
            bx::mtxLookAt(view, eye, at);

            float proj[16];
            bx::mtxProj(proj, 60.0f, static_cast<float>(render_width) / static_cast<float>(render_height),
                        0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

            bgfx::setViewTransform(0, view, proj);
        }

        bgfx::frame(); // send the frame
    }

    void shutdown()
    {
        bgfx::shutdown();
    }

    void resize(int width, int height)
    {
        bgfx::reset(width, height, BGFX_RESET_NONE);

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, width, height);

        render_height = height;
        render_width = width;
    }
};
