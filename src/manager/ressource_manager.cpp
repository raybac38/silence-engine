#include "ressource_manager.hpp"

extern "C"
{
    RessourceManager::Manager<Geometry::Mesh>::Handle ressource_manager_acquire_mesh(const char *path)
    {
        return RessourceManager::geometryManager.acquire(path);
    }

    void ressource_manager_release_mesh(RessourceManager::Manager<Geometry::Mesh>::Handle handle)
    {
        RessourceManager::geometryManager.release(handle);
    }
}