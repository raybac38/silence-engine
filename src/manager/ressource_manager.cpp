#include "ressource_manager.hpp"

extern "C"
{
    MeshHandle ressource_manager_acquire_mesh(const char *path)
    {
        return RessourceManager::geometryManager.acquire(path);
    }

    void ressource_manager_release_mesh(MeshHandle meshHandle)
    {
        RessourceManager::geometryManager.release(meshHandle);
    }
}