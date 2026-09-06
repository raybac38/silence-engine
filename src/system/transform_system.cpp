#include "transform_system.hpp"
#include <bgfx/bgfx.h>
#include "../config.hpp"
#include "../utils/sparse_set.tpp"

namespace
{
    SparseSet<TransformSystem::Transform> sparseSet;
}

void TransformSystem::attachTransform(size_t index)
{
    if (sparseSet.has(index))

        throw std::runtime_error("Transform system already attached");

    TransformSystem::Transform default_transform;
    default_transform.position = {0.0, 0.0, 0.0};
    default_transform.rotation = {0.0, 0.0, 0.0};
    default_transform.scale = {1.0, 1.0, 1.0};
    sparseSet.insert(index, default_transform);
}

void TransformSystem::removeTransform(size_t index)
{
    if (sparseSet.has(index))
    {
        sparseSet.delet(index);
    }
    else
    {
        throw std::runtime_error("Transform not attached to this entity");
    }
}

TransformSystem::Transform &TransformSystem::getTransform(size_t index)
{
    if (sparseSet.has(index))
    {
        return sparseSet.at(index);
    }
    else
    {
        throw std::runtime_error("Cannont acces, transform component not atteched");
    }
}

extern "C"
{
    void transform_system_attach_transform(size_t index)
    {
        TransformSystem::attachTransform(index);
    }

    void transform_system_remove_transform(size_t index)
    {
        TransformSystem::removeTransform(index);
    }

    TransformSystem::Transform *transform_system_get_transform(size_t index)
    {
        return &TransformSystem::getTransform(index);
    }
}
