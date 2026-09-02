#include "input_system.hpp"
#include <unordered_map>
#include <array>   // Provides std::array
#include <cstddef> // Provides std::size_t
#include <cstdio>

namespace InputSystem
{
    namespace
    {
        constexpr size_t MAX_KEYS = 512;
        std::array<bool, MAX_KEYS> raw_current_keys = {false};
        std::array<bool, MAX_KEYS> current_keys = {false};
        std::array<bool, MAX_KEYS> previous_keys = {false};
    };

    void processEvent(const SDL_Event &event)
    {
        printf("new event %d\n", event.key.key);
        if (event.type == SDL_EVENT_KEY_DOWN)
        {
            raw_current_keys[event.key.key] = true;
        }
        else if (event.type == SDL_EVENT_KEY_UP)
        {
            raw_current_keys[event.key.key] = false;
        }
    }

    void update()
    {
        previous_keys = current_keys;
        current_keys = raw_current_keys;
    }

    bool isHeld(int key)
    {
        return current_keys[key];
    }

    bool isPressed(int key)
    {
        return current_keys[key] && !previous_keys[key];
    }

    bool inRealeased(int key)
    {
        return !current_keys[key] && previous_keys[key];
    }
}

extern "C"
{
    bool input_is_held(int action_id)
    {
        return InputSystem::isHeld(action_id);
    }
    bool input_is_pressed(int action_id)
    {
        return InputSystem::isPressed(action_id);
    }
    bool input_is_released(int action_id)
    {
        return InputSystem::inRealeased(action_id);
    }
}