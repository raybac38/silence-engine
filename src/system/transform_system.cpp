#include "transform_system.hpp"
#include <bgfx/bgfx.h>
#include "../config.hpp"
#include "../utils/sparse_set.tpp"
#include "../utils/rotation.hpp"

namespace
{
    SparseSet<TransformSystem::Transform> sparseSet;

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

void TransformSystem::attachTransform(size_t index)
{
    if (sparseSet.has(index))

        throw std::runtime_error("Transform system already attached");

    TransformSystem::Transform default_transform;
    default_transform.position = {0.0, 0.0, 0.0};
    default_transform.rotation = Math::eulerToQuaternion({0.0, 0.0, 0.0});
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

void TransformSystem::setPosition(size_t index, Types::Vec3 position)
{
    Transform &current = getTransform(index);
    current.position = position;
}

void TransformSystem::translate(size_t index, Types::Vec3 vector)
{
    Transform &current = getTransform(index);
    current.position.x += vector.x;
    current.position.y += vector.y;
    current.position.z += vector.z;
}

void TransformSystem::setRotation(size_t index, Types::Vec3 rotation)
{
    Transform &current = getTransform(index);
    current.rotation = Math::eulerToQuaternion(rotation);
}

void TransformSystem::rotate(size_t index, Types::Vec3 vector)
{
    Transform &current = getTransform(index);
    Types::Vec4 rotationVector = Math::eulerToQuaternion(vector);
    Types::Vec4 combined = Math::combineRotations(current.rotation, rotationVector);
    current.rotation = Math::normalizeQuat(combined);
}

void TransformSystem::setScale(size_t index, Types::Vec3 scale)
{
    Transform &current = getTransform(index);
    current.scale = scale;
}

void TransformSystem::scale(size_t index, Types::Vec3 vector)
{
    Transform &current = getTransform(index);
    current.scale.x += vector.x;
    current.scale.y += vector.y;
    current.scale.z += vector.z;
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

    void transfrom_system_set_position(size_t index, Vec3 position)
    {
        TransformSystem::setPosition(index, position);
    }

    void transfrom_system_translate(size_t index, Vec3 vector)
    {
        TransformSystem::translate(index, vector);
    }

    void transfrom_system_set_rotation(size_t index, Vec3 rotation)
    {
        TransformSystem::setRotation(index, rotation);
    }

    void transfrom_system_rotate(size_t index, Vec3 vector)
    {
        TransformSystem::rotate(index, vector);
    }

    void transfrom_system_set_scale(size_t index, Vec3 scale)
    {
        TransformSystem::setScale(index, scale);
    }

    void transfrom_system_scale(size_t index, Vec3 vector)
    {
        TransformSystem::scale(index, vector);
    }
}
