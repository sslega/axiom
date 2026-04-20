#pragma once

#include "Scene/Component.h"
#include "Renderer/Camera.h"

namespace axiom
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent(float fovYRadians, float aspectRatio, float near, float far);
        CameraComponent(float left, float right, float bottom, float top, float near, float far);
        Camera m_camera;
    };
}