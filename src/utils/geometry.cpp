#include "geometry.h"
#include "types.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace
{
    // Fonction helper privée pour lire un sommet de face
    bool parseFaceIndices(const std::string &token, int &p, int &u, int &n)
    {
        p = u = n = 0;

        // Cas : v/vt/vn
        if (std::sscanf(token.c_str(), "%d/%d/%d", &p, &u, &n) == 3)
            return true;
        // Cas : v//vn (pas de texture)
        if (std::sscanf(token.c_str(), "%d//%d", &p, &n) == 2)
            return true;
        // Cas : v/vt (pas de normale)
        if (std::sscanf(token.c_str(), "%d/%d", &p, &u) == 2)
            return true;
        // Cas : v (uniquement la position)
        if (std::sscanf(token.c_str(), "%d", &p) == 1)
            return true;

        return false;
    }

    // Helper pour fabriquer le Vertex final
    Geometry::Vertex buildVertex(int pIdx, int uIdx, int nIdx,
                                 const std::vector<Types::Vec3> &pos,
                                 const std::vector<Types::Vec2> &uvs,
                                 const std::vector<Types::Vec3> &norms)
    {
        Geometry::Vertex v{};

        if (pIdx > 0 && pIdx <= static_cast<int>(pos.size()))
        {
            const auto &p = pos[pIdx - 1];
            v.x = p.x;
            v.y = p.y;
            v.z = p.z;
        }
        if (uIdx > 0 && uIdx <= static_cast<int>(uvs.size()))
        {
            const auto &u = uvs[uIdx - 1];
            v.u = u.x;
            v.v = u.y;
        }
        if (nIdx > 0 && nIdx <= static_cast<int>(norms.size()))
        {
            const auto &n = norms[nIdx - 1];
            v.nx = n.x;
            v.ny = n.y;
            v.nz = n.z;
        }

        return v;
    }
}

Geometry::Mesh::Mesh(const std::string &path)
{
    std::ifstream fichier(path);
    if (!fichier.is_open())
    {
        std::cerr << "[OBJ LOADER] Impossible d'ouvrir le fichier : " << path << std::endl;
        return;
    }

    std::vector<Types::Vec3> rawPosition;
    std::vector<Types::Vec3> rawNormal;
    std::vector<Types::Vec2> rawUv;

    std::string ligne;
    while (std::getline(fichier, ligne))
    {
        std::stringstream ss(ligne);
        std::string prefix;
        ss >> prefix;

        if (prefix == "v")
        {
            Types::Vec3 p;
            ss >> p.x >> p.y >> p.z;
            rawPosition.push_back(p);
        }
        else if (prefix == "vt")
        {
            Types::Vec2 u;
            ss >> u.x >> u.y;
            rawUv.push_back(u);
        }
        else if (prefix == "vn")
        {
            Types::Vec3 n;
            ss >> n.x >> n.y >> n.z;
            rawNormal.push_back(n);
        }
        else if (prefix == "f")
        {
            std::string token;
            while (ss >> token)
            {
                int pIdx = 0, uIdx = 0, nIdx = 0;
                if (parseFaceIndices(token, pIdx, uIdx, nIdx))
                {
                    vertices.push_back(buildVertex(pIdx, uIdx, nIdx, rawPosition, rawUv, rawNormal));
                }
            }
        }
    }

    return;
}