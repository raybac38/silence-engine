#include "shader_loader.hpp"
#include "bgfx/bgfx.h"
#include <fstream>
#include <vector>
#include <iostream>

namespace ShaderLoader
{
    bgfx::ShaderHandle loadShader(const char *filePath)
    {
        std::ifstream file(filePath, std::ios::binary | std::ios::ate);
        if (!file.is_open())
        {
            std::cerr << "Impossible d'ouvrir le shader : " << filePath << std::endl;
            return BGFX_INVALID_HANDLE;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);

        // Allocateur mémoire bgfx
        const bgfx::Memory *mem = bgfx::alloc(static_cast<uint32_t>(size));
        file.read(reinterpret_cast<char *>(mem->data), size);
        file.close();

        bgfx::ShaderHandle handle = bgfx::createShader(mem);
        return handle;
    }
}