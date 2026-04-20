#pragma once

#include "Scene/Component.h"
#include "Math/Vector.h"
#include "Math/Matrix4.h"

namespace axiom
{
    struct TransformComponent : public Component
    {
        Vec3 position {0.0f, 0.0f, 0.0f};
        Vec3 rotation {0.0f, 0.0f, 0.0f};
        Vec3 scale {1.0f, 1.0f, 1.0f};

        inline Matrix4 GetTransform() const
        {
            return Matrix4::Translate(position)
                * Matrix4::Rotate({1,0,0}, rotation.x)
                * Matrix4::Rotate({0,1,0}, rotation.y)
                * Matrix4::Rotate({0,0,1}, rotation.z)
                * Matrix4::Scale(scale);
        }

        Vec3 LocalToWorld(Vec3 localDir) const
        {
            Matrix4 rot = Matrix4::Rotate({1,0,0}, rotation.x)
                        * Matrix4::Rotate({0,1,0}, rotation.y)
                        * Matrix4::Rotate({0,0,1}, rotation.z);
            return {
                rot(0,0)*localDir.x + rot(0,1)*localDir.y + rot(0,2)*localDir.z,
                rot(1,0)*localDir.x + rot(1,1)*localDir.y + rot(1,2)*localDir.z,
                rot(2,0)*localDir.x + rot(2,1)*localDir.y + rot(2,2)*localDir.z
            };
        }

        Vec3 Forward() const { return LocalToWorld({ 0,  0, -1}); }
        Vec3 Right()   const { return LocalToWorld({ 1,  0,  0}); }
        Vec3 Up()      const { return LocalToWorld({ 0,  1,  0}); }
    };
}