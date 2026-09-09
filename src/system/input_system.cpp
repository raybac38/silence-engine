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

        float buffered_mouse_delta_position_x = 0.0f;
        float buffered_mouse_delta_position_y = 0.0f;
        float mouse_position_x = 0.0f;
        float mouse_position_y = 0.0f;
        float mouse_delta_position_x = 0.0f;
        float mouse_delta_position_y = 0.0f;

        float buffered_mouse_vertical_scroll = 0.0f;
        float buffered_mouse_horizontal_scroll = 0.0f;
        float mouse_vertical_scroll = 0.0f;
        float mouse_horizontal_scroll = 0.0f;
    };

    void processEvent(const SDL_Event &event)
    {
        if (event.type == SDL_EVENT_KEY_DOWN && event.key.key < MAX_KEYS)
        {
            raw_current_keys[event.key.key] = true;
        }
        else if (event.type == SDL_EVENT_KEY_UP && event.key.key < MAX_KEYS)
        {
            raw_current_keys[event.key.key] = false;
        }

        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            mouse_position_x = event.motion.x;
            mouse_position_y = event.motion.y;
            buffered_mouse_delta_position_x += event.motion.xrel;
            buffered_mouse_delta_position_y += event.motion.yrel;
        }
        if (event.type == SDL_EVENT_MOUSE_MOTION)
        {
            buffered_mouse_horizontal_scroll += event.wheel.x;
            buffered_mouse_vertical_scroll += event.wheel.y;
        }
    }

    void update()
    {
        mouse_delta_position_x = buffered_mouse_delta_position_x;
        mouse_delta_position_y = buffered_mouse_delta_position_y;
        mouse_horizontal_scroll = buffered_mouse_horizontal_scroll;
        mouse_vertical_scroll = buffered_mouse_vertical_scroll;

        buffered_mouse_delta_position_y = 0.0f;
        buffered_mouse_delta_position_x = 0.0f;
        buffered_mouse_horizontal_scroll = 0.0f;
        buffered_mouse_vertical_scroll = 0.0f;

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

    Types::Vec2 input_get_mouse_delta()
    {
        return Types::Vec2{InputSystem::mouse_delta_position_x, InputSystem::mouse_delta_position_y};
    }

    Types::Vec2 input_get_mouse_position()
    {
        return Types::Vec2{InputSystem::mouse_position_x, InputSystem::mouse_position_y};
    }

    Types::Vec2 input_get_mouse_scroll()
    {
        return Types::Vec2{InputSystem::mouse_horizontal_scroll, InputSystem::mouse_vertical_scroll};
    }
}