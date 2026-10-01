#pragma once

#include <vector>
#include <bx/math.h>
#include "../utils/sparse_set.tpp"
#include "../utils/types.hpp"
#include "../manager/entity_manager.hpp"

namespace TransformSystem
{
    struct Transform
    {
        Types::Vec3 position;
        Types::Vec4 rotation;
        Types::Vec3 scale;
    };

    void attachTransform(EntityManager::EntityId entityId);

    void removeTransform(EntityManager::EntityId entityId);

    TransformSystem::Transform &getTransform(EntityManager::EntityId entityId);

    void setPosition(EntityManager::EntityId entityId, Types::Vec3 position);

    void translate(EntityManager::EntityId entityId, Types::Vec3 vector);

    void setRotation(EntityManager::EntityId entityId, Types::Vec3 rotation);

    void rotate(EntityManager::EntityId entityId, Types::Vec3 vector);

    void setScale(EntityManager::EntityId entityId, Types::Vec3 scale);

    void scale(EntityManager::EntityId entityId, Types::Vec3 vector);
};

using Vec3 = Types::Vec3;

extern "C"
{
    void transform_system_attach_transform(EntityManager::EntityId entityId);

    void transform_system_remove_transform(EntityManager::EntityId entityId);

    void transfrom_system_set_position(EntityManager::EntityId entityId, Vec3 position);

    void transfrom_system_translate(EntityManager::EntityId entityId, Vec3 vector);

    void transfrom_system_set_rotation(EntityManager::EntityId entityId, Vec3 rotation);

    void transfrom_system_rotate(EntityManager::EntityId entityId, Vec3 vector);

    void transfrom_system_set_scale(EntityManager::EntityId entityId, Vec3 scale);

    void transfrom_system_scale(EntityManager::EntityId entityId, Vec3 vector);
}
