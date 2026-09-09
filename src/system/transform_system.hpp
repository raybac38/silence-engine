#pragma once

#include <vector>
#include <bx/math.h>
#include "../utils/sparse_set.tpp"
#include "../utils/types.hpp"

namespace TransformSystem
{
    struct Transform
    {
        Types::Vec3 position;
        Types::Vec4 rotation;
        Types::Vec3 scale;
    };

    void attachTransform(size_t index);

    void removeTransform(size_t index);

    TransformSystem::Transform &getTransform(size_t index);

    void setPosition(size_t index, Types::Vec3 position);

    void translate(size_t index, Types::Vec3 vector);

    void setRotation(size_t index, Types::Vec3 rotation);

    void rotate(size_t index, Types::Vec3 vector);

    void setScale(size_t index, Types::Vec3 scale);

    void scale(size_t index, Types::Vec3 vector);

};

using Vec3 = Types::Vec3;

extern "C"
{
    void transform_system_attach_transform(size_t index);

    void transform_system_remove_transform(size_t index);

    void transfrom_system_set_position(size_t index, Vec3 position);

    void transfrom_system_translate(size_t index, Vec3 vector);

    void transfrom_system_set_rotation(size_t index, Vec3 rotation);

    void transfrom_system_rotate(size_t index, Vec3 vector);

    void transfrom_system_set_scale(size_t index, Vec3 scale);

    void transfrom_system_scale(size_t index, Vec3 vector);

}
