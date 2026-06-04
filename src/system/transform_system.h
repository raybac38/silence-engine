#ifndef TRANSFORM_SYSTEM_H
#define TRANSFORM_SYSTEM_H

#include <vector>
#include <bx/math.h>
#include "../utils/sparse_set.h"

class TransformSystem
{
public:
    struct transformSystem
    {
        bx::Vec3 position;
        bx::Vec3 rotation;
        bx::Vec3 scale;
    };

    TransformSystem();

    size_t add();

    void remove(size_t index);

private:
    SparseSet<struct transformSystem> tranformsSparseSet;
    std::vector<size_t> entityToTransformIndex;
};

#endif