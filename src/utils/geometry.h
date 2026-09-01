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

        explicit Mesh(const std::string &path);
    };
}