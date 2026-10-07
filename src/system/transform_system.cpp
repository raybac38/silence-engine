#include "transform_system.hpp"
#include <bgfx/bgfx.h>
#include "../config.hpp"
#include "../utils/sparse_set.tpp"
#include "../utils/rotation.hpp"

namespace
{
    SparseSet<TransformSystem::Transform> sparseSet;
}
namespace Math
{
    // Produit de Hamilton.  a * b  =  "applique b, PUIS a".
    inline Types::Vec4 quatMul(const Types::Vec4 &a, const Types::Vec4 &b)
    {
        Types::Vec4 r;
        r.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
        r.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
        r.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
        r.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
        return r;
    }

    inline Types::Vec4 quatIdentity()
    {
        Types::Vec4 q;
        q.x = 0;
        q.y = 0;
        q.z = 0;
        q.w = 1;
        return q;
    }

    inline Types::Vec4 quatNormalize(const Types::Vec4 &q)
    {
        float len = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);
        if (len < 1e-8f)
            return quatIdentity();
        Types::Vec4 r;
        r.x = q.x / len;
        r.y = q.y / len;
        r.z = q.z / len;
        r.w = q.w / len;
        return r;
    }

    // Quaternion depuis un "vecteur de rotation" :
    //   direction = axe de rotation, norme = angle (radians).
    // Aucun ordre d'axes impliqué -> pas de gimbal lock, pas d'ambiguïté d'Euler.
    // Idéal pour intégrer une vitesse angulaire : v = omega * dt.
    inline Types::Vec4 quatFromRotationVector(const Types::Vec3 &v)
    {
        float angle = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (angle < 1e-8f)
            return quatIdentity();
        float s = std::sin(angle * 0.5f) / angle; // axe normalisé * sin(angle/2)
        Types::Vec4 q;
        q.x = v.x * s;
        q.y = v.y * s;
        q.z = v.z * s;
        q.w = std::cos(angle * 0.5f);
        return q;
    }

    // Applique la rotation q (unitaire) au vecteur v  (v' = q v q*).
    // Sert à récupérer les axes locaux : avant = quatRotate(q, {0, 0, 1}), etc.
    inline Types::Vec3 quatRotate(const Types::Vec4 &q, const Types::Vec3 &v)
    {
        // t = 2 * cross(q.xyz, v)
        float tx = 2.0f * (q.y * v.z - q.z * v.y);
        float ty = 2.0f * (q.z * v.x - q.x * v.z);
        float tz = 2.0f * (q.x * v.y - q.y * v.x);
        // v' = v + w * t + cross(q.xyz, t)
        Types::Vec3 r;
        r.x = v.x + q.w * tx + (q.y * tz - q.z * ty);
        r.y = v.y + q.w * ty + (q.z * tx - q.x * tz);
        r.z = v.z + q.w * tz + (q.x * ty - q.y * tx);
        return r;
    }
}

void TransformSystem::rotate(EntityManager::EntityId entityId, Types::Vec3 angles)
{
    Transform &current = getTransform(entityId);
    Types::Vec4 delta = Math::quatFromRotationVector(angles);
    // q * delta  ->  delta est exprimé dans le repère de l'objet
    current.rotation = Math::quatNormalize(Math::quatMul(current.rotation, delta));
}

// Rotation autour des axes FIXES du monde.
void TransformSystem::rotateWorld(EntityManager::EntityId entityId, Types::Vec3 angles)
{
    Transform &current = getTransform(entityId);
    Types::Vec4 delta = Math::quatFromRotationVector(angles);
    // delta * q  ->  delta est exprimé dans le repère du monde
    current.rotation = Math::quatNormalize(Math::quatMul(delta, current.rotation));
}

// Contrôle "souris" : yaw autour du Y MONDE, pitch autour du X LOCAL.
// L'horizon reste toujours droit (aucun roulis ne s'accumule).
// yaw et pitch en radians, incréments par frame.
void TransformSystem::rotateYawPitch(EntityManager::EntityId entityId, float yaw, float pitch)
{
    Transform &current = getTransform(entityId);
    Types::Vec4 qYaw = Math::quatFromRotationVector({0.0f, yaw, 0.0f});
    Types::Vec4 qPitch = Math::quatFromRotationVector({pitch, 0.0f, 0.0f});
    // yaw * q * pitch : yaw appliqué dans le monde, pitch dans le repère local
    Types::Vec4 q = Math::quatMul(qYaw, Math::quatMul(current.rotation, qPitch));
    current.rotation = Math::quatNormalize(q);
}

// Déplacement le long des axes locaux (avancer "vers le nez" du vaisseau).
// vector = {droite, haut, avant} dans le repère de l'entité.
void TransformSystem::translateLocal(EntityManager::EntityId entityId, Types::Vec3 vector)
{
    Transform &current = getTransform(entityId);
    Types::Vec3 world = Math::quatRotate(current.rotation, vector);
    current.position.x += world.x;
    current.position.y += world.y;
    current.position.z += world.z;
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

// void TransformSystem::rotate(EntityManager::EntityId entityId, Types::Vec3 vector)
// {
//     Transform &current = getTransform(entityId);
//     Types::Vec4 rotationVector = Math::eulerToQuaternion(vector);
//     Types::Vec4 combined = Math::combineRotations(current.rotation, rotationVector);
//     current.rotation = Math::normalizeQuat(combined);
// }
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

    void transform_system_rotate_yaw_pitch(EntityManager::EntityId entityId, float yaw, float pitch)
    {
        TransformSystem::rotateYawPitch(entityId, yaw, pitch);
    }

    void transform_system_rotate_world(EntityManager::EntityId entityId, Vec3 vector)
    {
        TransformSystem::rotateWorld(entityId, vector);
    }

    void transform_system_translate_local(EntityManager::EntityId entityId, Vec3 vector)
    {
        TransformSystem::translateLocal(entityId, vector);
    }
}
