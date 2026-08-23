#pragma once
#include <bgfx/bgfx.h>

namespace ShaderLoader
{
    bgfx::ShaderHandle loadShader(const char *filePath);
};