#ifndef CORE_H
#define CORE_H

#include "manager/window_manager.h"

class Core : public WindowManager::WindowEventListener
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