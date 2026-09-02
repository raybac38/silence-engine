#pragma once
#include <SDL3/SDL.h>

namespace InputSystem
{

    enum class KeyState
    {
        NONE,
        PRESSED,
        HELP,
        RELEASED
    };

    void processEvent(const SDL_Event &event);

    void update();

    bool isHeld(int key);

    bool isPressed(int key);

    bool inRealeased(int key);

}

extern "C"
{
    bool input_is_held(int action_id);
    bool input_is_pressed(int action_id);
    bool input_is_released(int action_id);
}