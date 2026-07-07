#pragma once

#include "Math/Vector.h"
#include "Renderer/Camera.h"

namespace axiom
{

    class EditorCamera
    {
    public:
        EditorCamera();

        void OnUpdate(float deltaTime);
        inline void SetAspectRatio(float aspectRatio) { m_camera.SetAspectRatio(aspectRatio); }
        inline Camera& GetCamera() { return m_camera; }

    private:
        Camera m_camera;
        float m_yaw = 0.0f;
        float m_pitch = 0.0f;

        Vec2 m_lastMouse = {0, 0};
        float m_moveSpeed = 5.0f;
        float m_lookSensitivity = 0.002f;
    };

}