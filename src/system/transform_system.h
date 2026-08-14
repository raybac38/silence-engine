#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include <vector>
#include <bx/math.h>
#include "../utils/sparse_set.tpp"
#include "../utils/types.h"

namespace TransformSystem
{
    struct Transform
    {
        Types::Vec3 position;
        Types::Vec3 rotation;
        Types::Vec3 scale;
    };

    void attachTransform(size_t index);

    void removeTransform(size_t index);

    TransformSystem::Transform &getTransform(size_t index);
};

extern "C"
{
    void transform_system_attach_transform(size_t index);

    void transform_system_remove_transform(size_t index);

    TransformSystem::Transform *transform_system_get_transform(size_t index);
}

#endif
