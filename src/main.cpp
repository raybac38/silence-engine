#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <SDL3/SDL.h>
#include <cstdio>

// Optionnel : inclus ton système si besoin
// #include "system/transform_system.h" 

int main(int argc, char* argv[])
{
    // 1. Initialisation de SDL3
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    // 2. Création de la fenêtre
    // Note: Pour OpenGL avec bgfx, il est souvent préférable de laisser SDL savoir qu'on peut utiliser OpenGL
    SDL_Window* window = SDL_CreateWindow(
        "Silence",
        1280,
        720,
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        std::printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 3. Configuration des données de plateforme (bgfx::PlatformData)
    bgfx::PlatformData pd{};
    SDL_PropertiesID props = SDL_GetWindowProperties(window);
    const char* video_driver = SDL_GetCurrentVideoDriver();

    std::printf("Driver vidéo SDL3 détecté : %s\n", video_driver);

    if (SDL_strcmp(video_driver, "wayland") == 0)
    {
        pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER, nullptr);
        pd.nwh = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER, nullptr);
    }
    else if (SDL_strcmp(video_driver, "x11") == 0)
    {
        pd.ndt = SDL_GetPointerProperty(props, SDL_PROP_WINDOW_X11_DISPLAY_POINTER, nullptr);
        // Attention : sous X11, le Window ID est un entier, on le cast en void* pour bgfx
        pd.nwh = reinterpret_cast<void*>(SDL_GetNumberProperty(props, SDL_PROP_WINDOW_X11_WINDOW_NUMBER, 0));
    }
    else 
    {
        std::printf("Driver non géré explicitement ou autre OS (Windows/macOS).\n");
        // Sur Windows (Windows/DirectX/OpenGL), SDL_PROP_WINDOW_WIN32_HWND_POINTER serait utilisé
    }

    std::printf("Données plateforme définies -> ndt: %p, nwh: %p\n", pd.ndt, pd.nwh);

    if (!pd.nwh)
    {
        std::printf("Erreur : Impossible de récupérer le handle de la fenêtre (nwh est nul).\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 4. Initialisation de bgfx
    bgfx::Init init{};
    init.type = bgfx::RendererType::Count; // Tu peux utiliser RendererType::Count pour de l'auto-détection
    init.platformData = pd;
    init.resolution.width = 1280;
    init.resolution.height = 720;
    init.resolution.reset = BGFX_RESET_VSYNC;

    if (!bgfx::init(init))
    {
        std::printf("Erreur : Échec de l'initialisation de bgfx.\n");
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Définir la vue par défaut
    bgfx::setViewClear(0, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x303030ff, 1.0f, 0);
    bgfx::setViewRect(0, 0, 0, 1280, 720);

    // 5. Boucle Principale
    bool quit = false;
    while (!quit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                quit = true;
            }
            else if (event.type == SDL_EVENT_WINDOW_RESIZED)
            {
                // Gérer le redimensionnement dans bgfx
                bgfx::reset(event.window.data1, event.window.data2, BGFX_RESET_VSYNC);
                bgfx::setViewRect(0, 0, 0, event.window.data1, event.window.data2);
            }
        }

        // Indiquer à bgfx qu'on utilise la vue 0
        bgfx::touch(0);

        // Soumettre le rendu de la frame
        bgfx::frame();
    }

    // 6. Nettoyage
    bgfx::shutdown();
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
