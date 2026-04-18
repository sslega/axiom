#include "CameraComponent.h"

namespace axiom
{
    CameraComponent::CameraComponent(float left, float right, float bottom, float top)
    {
        m_camera = OrtographicCamera(left, right, bottom, top);
    }
}