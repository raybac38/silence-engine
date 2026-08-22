#ifndef CORE_H
#define CORE_H

#include "manager/window_manager.h"

namespace Core
{
    /**
    @breif Initialise the game
    */
    void init();
    /**
     * @breif Run the gameloop
     */
    void run();
    /**
     * @brif Shutdown the gameloop
     */
    void shutdown();
};

#endif