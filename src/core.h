#ifndef CORE_H
#define CORE_H

#include "window_system.h"

class Core : public WindowSystem::WindowEventListener
{
public:
    Core();
    void run();

    // WindowEventListener

    void onQuit() override;
    void onResize(int width, int height) override;

private:
    bool quit;
};

#endif