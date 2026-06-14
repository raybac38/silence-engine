#include "window_system.h"

#include <SDL3/SDL.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <algorithm>

WindowSystem::~WindowSystem()
{
    if (this->window)
        SDL_DestroyWindow(this->window);
    SDL_Quit();
}

bool WindowSystem::init(const std::string &title, int width, int height)
{
    this->width = width;
    this->height = height;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        std::cerr << "[ERROR] SDL_Init : " << SDL_GetError() << std::endl;
        return false;
    }
    std::cout << "[INFO] SDL init" << std::endl;

    this->window = SDL_CreateWindow(title.c_str(), width, height, SDL_WINDOW_RESIZABLE);

    if (!this->window)
    {
        std::cerr << "[ERROR] SDL_CreateWindow : " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }
    std::cout << "[INFO] SDL create window" << std::endl;
    return true;
}

bool WindowSystem::isOpen()
{
    return this->window != nullptr;
}

SDL_Window *WindowSystem::getWindow()
{
    return this->window;
}

SDL_PropertiesID WindowSystem::getProperties()
{
    return SDL_GetWindowProperties(window);
}

int WindowSystem::getHeight()
{
    return this->height;
}

int WindowSystem::getWidth()
{
    return this->width;
}

void WindowSystem::pollEvent()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_EVENT_QUIT:
            for (auto *listener : windowEventListeners)
            {
                listener->onQuit();
            }
            break;
        case SDL_EVENT_WINDOW_RESIZED:
            for (auto *listener : windowEventListeners)
            {
                listener->onResize(event.window.data1, event.window.data2);
            }
            break;

        case SDL_EVENT_KEY_DOWN:
            for (auto *listener : keyListeners)
            {
                listener->onKeyPressed(event.key.key);
            }
            break;

        case SDL_EVENT_MOUSE_MOTION:
            for (auto *listener : mouseListeners)
            {
                listener->onMouseMove(event.motion.x, event.motion.y);
            }
            break;
        }
    }
}

void WindowSystem::addListener(KeyListener *listener)
{
    if (listener)
        this->keyListeners.push_back(listener);
}

void WindowSystem::addListener(MouseListener *listener)
{
    if (listener)
        this->mouseListeners.push_back(listener);
}

void WindowSystem::addListener(WindowEventListener *listener)
{
    if (listener)
        this->windowEventListeners.push_back(listener);
}

void WindowSystem::removeListener(KeyListener *listener)
{
    if (listener)
        keyListeners.erase(std::remove(keyListeners.begin(), keyListeners.end(), listener), keyListeners.end());
}

void WindowSystem::removeListener(MouseListener *listener)
{
    if (listener)
        mouseListeners.erase(std::remove(mouseListeners.begin(), mouseListeners.end(), listener), mouseListeners.end());
}

void WindowSystem::removeListener(WindowEventListener *listener)
{
    if (listener)
        windowEventListeners.erase(std::remove(windowEventListeners.begin(), windowEventListeners.end(), listener), windowEventListeners.end());
}
