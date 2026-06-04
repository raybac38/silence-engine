#include "transform_system.h"
#include <bgfx/bgfx.h>
#include "../config.h"
#include "../utils/sparse_set.h"

TransformSystem::TransformSystem()
{
    this->tranformsSparseSet = SparseSet<struct transformSystem>();
}

size_t TransformSystem::add()
{
    bx::Vec3 default_position = {0.0f, 0.0f, 0.0f};
    bx::Vec3 default_rotation = {0.0f, 0.0f, 0.0f};
    bx::Vec3 default_scale = {1.0f, 1.0f, 1.0f};

    size_t index = this->tranformsSparseSet.insert({default_position, default_rotation, default_scale});

    return index;
}

void TransformSystem::remove(size_t index)
{
    this->tranformsSparseSet.delet(index);
}
