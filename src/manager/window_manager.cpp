#include "window_manager.h"

#include <SDL3/SDL.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>

namespace WindowManager
{
    namespace
    {
        int window_width = 0;
        int window_height = 0;
        SDL_Window *window = nullptr;
    }

    bool open(const std::string &title, int width, int height)
    {
        window_width = width;
        window_height = height;

        if (!SDL_Init(SDL_INIT_VIDEO))
        {
            std::cerr << "[ERROR] SDL_Init : " << SDL_GetError() << std::endl;
            return false;
        }
        std::cout << "[INFO] SDL init" << std::endl;

        window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);

        if (!window)
        {
            std::cerr << "[ERROR] SDL_CreateWindow : " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }
        std::cout << "[INFO] SDL create window" << std::endl;
        return true;
    }

    void close()
    {
        if (isOpen())
            SDL_DestroyWindow(window);
        window = nullptr;
        SDL_Quit();
    }

    bool isOpen()
    {
        return window != nullptr;
    }

    SDL_Window *getWindow()
    {
        return window;
    }

    SDL_PropertiesID getProperties()
    {
        return SDL_GetWindowProperties(window);
    }

    void poolEvent()
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                //
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                //
                break;

            case SDL_EVENT_KEY_DOWN:
                //
                break;

            case SDL_EVENT_MOUSE_MOTION:
                //
                break;
            }
        }
    }

    int getWidth()
    {
        return window_width;
    }

    int getHeight()
    {
        return window_height;
    }
};
