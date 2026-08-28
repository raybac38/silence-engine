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
    void freeMesh(Mesh *mesh);
}

extern "C"
{
    Geometry::Mesh geometry_load_file(const char *path);
    void geometry_free_mesh(Geometry::Mesh *mesh);
}