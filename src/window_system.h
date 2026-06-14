#ifndef WINDOW_SYSTEM_H
#define WINDOW_SYSTEM_H
#include <memory>
#include <string>

#include <SDL3/SDL.h>
#include <vector>
#include <optional>

class WindowSystem
{
public:
    class KeyListener
    {
    public:
        virtual ~KeyListener() = default;
        virtual void onKeyPressed(SDL_Keycode key) = 0;
        virtual void onKeyReleased(SDL_Keycode key) = 0;
    };

    class MouseListener
    {
    public:
        virtual ~MouseListener() = default;
        virtual void onMouseMove(int x, int y) = 0;
        virtual void onMouseButton(uint8_t button, bool pressed) = 0;
    };

    class WindowEventListener
    {
    public:
        virtual ~WindowEventListener() = default;
        virtual void onQuit() = 0;
        virtual void onResize(int width, int height) = 0;
    };
    ~WindowSystem();

    bool init(const std::string &title, int width, int height);
    bool isOpen();
    SDL_Window *getWindow();
    SDL_PropertiesID getProperties();

    void pollEvent();

    void addListener(KeyListener * listener);
    void addListener(MouseListener * listener);
    void addListener(WindowEventListener * listener);
    void removeListener(KeyListener * listener);
    void removeListener(MouseListener * listener);
    void removeListener(WindowEventListener * listener);

    int getWidth();
    int getHeight();

private:
    int width;
    int height;
    SDL_Window *window;
    std::vector<KeyListener *> keyListeners;
    std::vector<MouseListener *> mouseListeners;
    std::vector<WindowEventListener *> windowEventListeners;
};
#endif