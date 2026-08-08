#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include <vector>
#include <bx/math.h>
#include "../utils/sparse_set.tpp"

class TransformSystem
{
public:
    struct Transform
    {
        bx::Vec3 position;
        bx::Vec3 rotation;
        bx::Vec3 scale;
    };

    TransformSystem();

    void add(size_t index);

    void remove(size_t index);

    TransformSystem::Transform& seek(size_t index);

private:
    SparseSet<Transform> tranformsSparseSet;
};

#endif
