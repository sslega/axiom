#pragma once

#include "Scene/Component.h"
#include "Renderer/Camera.h"

namespace axiom
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent() = default;
        CameraComponent(float left, float right, float bottom, float top);
        OrtographicCamera m_camera;
    };
}