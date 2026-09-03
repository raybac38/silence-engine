#include "render_system.h"
#include "../utils/shader_loader.h"

#include <SDL3/SDL.h>
#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <bgfx/platform.h>
#include "../manager/window_manager.h"
#include "transform_system.h"
#include "../utils/sparse_set.tpp"
#include <chrono>
#include <iostream>

namespace RenderSystem
{
    namespace
    {
        struct RenderComponent
        {
            size_t entity_id;
            bgfx::VertexBufferHandle vbh = BGFX_INVALID_HANDLE; // Vertex Buffer
            bgfx::IndexBufferHandle ibh = BGFX_INVALID_HANDLE;
            // Index Buffer
            bgfx::ProgramHandle program = BGFX_INVALID_HANDLE;
            // Material
        };

        int render_width,
            render_height;

        SparseSet<RenderComponent> sparseSet;

        bgfx::VertexLayout vertexLayout;

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
        
        SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "wayland,x11");
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

        render_width = WindowManager::getWidth();
        render_height = WindowManager::getHeight();

        init.resolution.width = render_width;
        init.resolution.height = render_height;
        init.resolution.reset = BGFX_RESET_VSYNC; // Put BGFX_RESET_NONE if want uncapped fps

        if (!bgfx::init(init))
        {
            std::cerr << "[ERROR] RenderSystem : failed initiate BGFX" << std::endl;
            return false;
        }

        bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
        bgfx::setViewRect(0, 0, 0, render_width, render_height);

        // This is the default vertex layout from utils/geometry.h
        vertexLayout.begin()
            .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
            .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
            .end();

        return true;
    }

    void render()
    {
        bgfx::touch(0); // submit empty to the fram, that indirectly clear the window

        const bx::Vec3 at = {0.0f, 0.0f, 0.0f};
        const bx::Vec3 eye = {0.0f, 0.0f, -5.0f};

        {
            float view[16];
            bx::mtxLookAt(view, eye, at);

            float proj[16];
            bx::mtxProj(proj, 60.0f, static_cast<float>(render_width) / static_cast<float>(render_height),
                        0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

            bgfx::setViewTransform(0, view, proj);
        }

        uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA;

        if (sparseSet.size() > 0)
        {
            std::vector<RenderComponent> &components = sparseSet.data();
            for (RenderComponent &comp : components)
            {
                // TODO : This can cause the game to crash if no transfom is available
                TransformSystem::Transform &transform = TransformSystem::getTransform(comp.entity_id);

                float rMtx[16];
                bx::Quaternion quat = bx::fromEuler({transform.rotation.x, transform.rotation.y, transform.rotation.z});

                bx::mtxFromQuaternion(rMtx, quat);

                float tMtx[16];
                bx::mtxTranslate(tMtx, transform.position.x, transform.position.y, transform.position.z);

                float mtx[16];
                bx::mtxMul(mtx, rMtx, tMtx);

                bgfx::setTransform(mtx);

                bgfx::setVertexBuffer(0, comp.vbh);
                bgfx::setIndexBuffer(comp.ibh);

                bgfx::setState(state);

                bgfx::submit(0, comp.program);
            }
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
    void attachMesh(size_t entityId, RessourceManager::Manager<Geometry::Mesh>::Handle handle)
    {
        if (sparseSet.has(entityId))
        {
            removeMesh(entityId);
        }
        RenderComponent component;
        component.entity_id = entityId;

        // Temporary

        bgfx::ShaderHandle vsh = ShaderLoader::loadShader("/home/ray/Projects/silence/silence-engine/shader/vs_default.sc");
        bgfx::ShaderHandle fsh = ShaderLoader::loadShader("/home/ray/Projects/silence/silence-engine/shader/fs_default.sc");

        component.program = bgfx::createProgram(vsh, fsh, true);

        component.ibh = BGFX_INVALID_HANDLE;

        const Geometry::Mesh *mesh = RessourceManager::geometryManager.get(handle);

        if (!mesh->vertices.empty())
        {
            uint32_t dataSize = static_cast<uint32_t>(mesh->vertices.size() * sizeof(Geometry::Vertex));
            const bgfx::Memory *mem = bgfx::copy(mesh->vertices.data(), dataSize);

            component.vbh = bgfx::createVertexBuffer(mem, vertexLayout);
        }

        sparseSet.insert(entityId, component);
    }
    void removeMesh(size_t entityId)
    {
        RenderComponent &component = sparseSet.at(entityId);

        if (bgfx::isValid(component.vbh))
        {
            bgfx::destroy(component.vbh);
            component.vbh = BGFX_INVALID_HANDLE;
        }

        if (bgfx::isValid(component.ibh))
        {
            bgfx::destroy(component.ibh);
            component.ibh = BGFX_INVALID_HANDLE;
        }
        sparseSet.delet(entityId);
    }
};

extern "C"
{
    void render_system_attach_mesh(size_t entityId, RessourceManager::Manager<Geometry::Mesh>::Handle handle)
    {
        RenderSystem::attachMesh(entityId, handle);
    }

    void render_system_remove_mesh(size_t entityId)
    {
        RenderSystem::removeMesh(entityId);
    }
}