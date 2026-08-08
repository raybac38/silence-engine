#include "transform_system.h"
#include <bgfx/bgfx.h>
#include "../config.h"
#include "../utils/sparse_set.tpp"

TransformSystem::TransformSystem()
{
    this->tranformsSparseSet = SparseSet<Transform>();
}

void TransformSystem::add(size_t entityId)
{
    bx::Vec3 default_position = {0.0f, 0.0f, 0.0f};
    bx::Vec3 default_rotation = {0.0f, 0.0f, 0.0f};
    bx::Vec3 default_scale = {1.0f, 1.0f, 1.0f};

    this->tranformsSparseSet.insert(entityId, {default_position, default_rotation, default_scale});
}

void TransformSystem::remove(size_t entityId)
{
    this->tranformsSparseSet.delet(entityId);
}

TransformSystem::Transform &TransformSystem::seek(size_t entityId){
  return this->tranformsSparseSet.at(entityId);
}
