#pragma once
#include <vector>
#include <string>

namespace Geometry
{
    struct Vertex
    {
        float x, y, z;
        float nx, ny, nz;
        float u, v;
    };

    struct Mesh
    {
        std::vector<Vertex> vertices;
    };

    Mesh loadFile(std::string path);
}