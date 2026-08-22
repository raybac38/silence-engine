#pragma once

#include <memory>
#include <string>

#include <SDL3/SDL.h>
#include <vector>
#include <optional>

namespace WindowManager
{
    /**
     * @brief Open a window
     * @param title title of the window
     * @param width width of the window
     * @param height height of the window
     */
    bool open(const std::string &title, int width, int height);

    /**
     * @brief Close the window and free memory
     */
    void close();

    /**
     * @brief Is the window open
     * @return does the is window openned
     */
    bool isOpen();

    /**
     *  Get window pointer
     */

    SDL_Window *getWindow();

    /**
     * @brief Get the current SDL properties
     */
    SDL_PropertiesID getProperties();

    /**
     * @brief Poll window event
     */
    void pollEvent();

    /**
     * @brief width of the window
     */
    int getWidth();
    /**
     * @brief heigth of the window
     */
    int getHeight();
};
