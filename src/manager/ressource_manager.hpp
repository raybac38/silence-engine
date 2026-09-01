#pragma once
#include <string>
#include <cstdint>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stack>
#include <stdexcept>
#include "../utils/geometry.h"

/**
 *
 * Principe des index
 *
 * premier uint32_t : generation
 * second uint32_t : index
 *
 */

namespace RessourceManager
{
    template <typename T>
    class Manager
    {
    public:
        struct Handle
        {
            uint32_t index = 0;
            uint32_t generation = 0;

            bool operator==(const Handle &other) const
            {
                return index == other.index && generation == other.generation;
            }
        };

        /**
         * @brief Aquire a ressource using a path
         * @param path path of the ressource to load
         */
        Handle acquire(const std::string &path)
        {
            auto it = path_to_handle.find(path);
            if (it != path_to_handle.end())
            {
                Slot &slot = slots[it->second.index];
                slot.ref_count++;
                return it->second;
            }

            uint32_t index;
            if (!free_indices.empty()) // Get a new index from the stack if possible
            {
                index = free_indices.top();
                free_indices.pop();
            }
            else
            {
                index = static_cast<uint32_t>(slots.size());
                slots.emplace_back();
            }

            Slot &slot = slots[index];
            slot.data = std::make_unique<T>(path);
            slot.ref_count = 1;
            slot.path = path;

            Handle handle{index, slot.generation};
            path_to_handle[path] = handle;

            return handle;
        }

        // Récupération de la ressource en $O(1)$ sécurisé
        const T *get(Handle handle) const
        {
            if (handle.index >= slots.size())
                return nullptr;

            const Slot &slot = slots[handle.index];

            if (slot.generation != handle.generation || !slot.data)
            {
                return nullptr;
            }

            return slot.data.get();
        }

        // Libération de la ressource
        void release(Handle handle)
        {
            if (handle.index >= slots.size())
                return;

            Slot &slot = slots[handle.index];
            if (slot.generation != handle.generation || !slot.data)
                return;

            slot.ref_count--;
            if (slot.ref_count == 0)
            {
                path_to_handle.erase(slot.path);
                slot.data.reset();
                slot.generation++;
                free_indices.push(handle.index);
            }
        }

    private:
        struct Slot
        {
            std::unique_ptr<T> data = nullptr;
            std::string path = "";
            uint32_t ref_count = 0;
            uint32_t generation = 1;
        };

        std::vector<Slot> slots;
        std::unordered_map<std::string, Handle> path_to_handle;
        std::stack<uint32_t> free_indices;
    };

    inline Manager<Geometry::Mesh> geometryManager;
}

extern "C"
{
    RessourceManager::Manager<Geometry::Mesh>::Handle ressource_manager_acquire_mesh(const char *path);
    void ressource_manager_release_mesh(RessourceManager::Manager<Geometry::Mesh>::Handle handle);
}