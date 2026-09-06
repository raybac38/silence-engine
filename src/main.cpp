#include "core.hpp"
#include <cstdio>

int main([[maybe_unused]] int argc, [[maybe_unused]] char *argv[])
{
    Core::init();
    Core::run();
}