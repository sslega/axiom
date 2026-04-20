#include "CameraComponent.h"

namespace axiom
{
    CameraComponent::CameraComponent(float fovYRadians, float aspectRatio, float near, float far)
    : m_camera(fovYRadians, aspectRatio, near, far)
    {
    }

    CameraComponent::CameraComponent(float left, float right, float bottom, float top, float near, float far)
    : m_camera(left, right, bottom, top, near, far)
    {
    }
}