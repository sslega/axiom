#pragma once
#include "Scene/Component.h"
#include "Math/Vector.h"

namespace axiom
{
    class CameraController: public Component
    {
        float moveSpeed      = 0.5f;
        float lookSensitivity = 0.002f;

        virtual void OnUpdate(float deltaTime) override;
    
    private:
        Vec2 m_lastMouse = {0, 0};
        bool m_firstUpdate = true;
    };
}