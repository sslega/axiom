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
    };
}