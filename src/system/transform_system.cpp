#include "transform_system.hpp"
#include <bgfx/bgfx.h>
#include "../config.hpp"
#include "../utils/sparse_set.tpp"
#include "../utils/rotation.hpp"

namespace
{
    SparseSet<TransformSystem::Transform> sparseSet;
}

TransformSystem::Transform &TransformSystem::getTransform(EntityManager::EntityId entityId)
{
    uint32_t id = entityId.id;
    if (sparseSet.has(id))
    {
        return sparseSet.at(id);
    }
    else
    {
        throw std::runtime_error("Cannont acces, transform component not atteched");
    }
}

void TransformSystem::attachTransform(EntityManager::EntityId entityId)
{
    uint32_t id = entityId.id;
    if (sparseSet.has(id))

        throw std::runtime_error("Transform system already attached");

    TransformSystem::Transform default_transform;
    default_transform.position = {0.0, 0.0, 0.0};
    default_transform.rotation = Math::eulerToQuaternion({0.0, 0.0, 0.0});
    default_transform.scale = {1.0, 1.0, 1.0};
    sparseSet.insert(id, default_transform);
}

void TransformSystem::removeTransform(EntityManager::EntityId entityId)
{
    uint32_t id = entityId.id;
    if (sparseSet.has(id))
    {
        sparseSet.delet(id);
    }
    else
    {
        throw std::runtime_error("Transform not attached to this entity");
    }
}

void TransformSystem::setPosition(EntityManager::EntityId entityId, Types::Vec3 position)
{
    Transform &current = getTransform(entityId);
    current.position = position;
}

void TransformSystem::translate(EntityManager::EntityId entityId, Types::Vec3 vector)
{
    Transform &current = getTransform(entityId);
    current.position.x += vector.x;
    current.position.y += vector.y;
    current.position.z += vector.z;
}

void TransformSystem::setRotation(EntityManager::EntityId entityId, Types::Vec3 rotation)
{
    Transform &current = getTransform(entityId);
    current.rotation = Math::eulerToQuaternion(rotation);
}

void TransformSystem::rotate(EntityManager::EntityId entityId, Types::Vec3 vector)
{
    Transform &current = getTransform(entityId);
    Types::Vec4 rotationVector = Math::eulerToQuaternion(vector);
    Types::Vec4 combined = Math::combineRotations(current.rotation, rotationVector);
    current.rotation = Math::normalizeQuat(combined);
}

void TransformSystem::setScale(EntityManager::EntityId entityId, Types::Vec3 scale)
{
    Transform &current = getTransform(entityId);
    current.scale = scale;
}

void TransformSystem::scale(EntityManager::EntityId entityId, Types::Vec3 vector)
{
    Transform &current = getTransform(entityId);
    current.scale.x += vector.x;
    current.scale.y += vector.y;
    current.scale.z += vector.z;
}

extern "C"
{
    void transform_system_attach_transform(EntityManager::EntityId entityId)
    {
        TransformSystem::attachTransform(entityId);
    }

    void transform_system_remove_transform(EntityManager::EntityId entityId)
    {
        TransformSystem::removeTransform(entityId);
    }

    void transfrom_system_set_position(EntityManager::EntityId entityId, Vec3 position)
    {
        TransformSystem::setPosition(entityId, position);
    }

    void transfrom_system_translate(EntityManager::EntityId entityId, Vec3 vector)
    {
        TransformSystem::translate(entityId, vector);
    }

    void transfrom_system_set_rotation(EntityManager::EntityId entityId, Vec3 rotation)
    {
        TransformSystem::setRotation(entityId, rotation);
    }

    void transfrom_system_rotate(EntityManager::EntityId entityId, Vec3 vector)
    {
        TransformSystem::rotate(entityId, vector);
    }

    void transfrom_system_set_scale(EntityManager::EntityId entityId, Vec3 scale)
    {
        TransformSystem::setScale(entityId, scale);
    }

    void transfrom_system_scale(EntityManager::EntityId entityId, Vec3 vector)
    {
        TransformSystem::scale(entityId, vector);
    }
}
