#include "EditorCamera.h"
#include "Math/Math.h"
#include "Input/Input.h"

namespace axiom
{
    axiom::EditorCamera::EditorCamera()
    {
        m_camera.SetView(Vec3(0,0,0), m_yaw, m_pitch);
    }
    
    void EditorCamera::OnUpdate(float deltaTime)
    {
        float dt = MillisecondsToSeconds(deltaTime);

        // Mouse look
        Vec2 mouse = Input::GetMousePosition();
        bool rmbPressed = Input::IsMouseButtonPressed(MouseCode::Right);
        Input::SetCursorLocked(rmbPressed);
        if(rmbPressed)
        {
            float dx = mouse.x - m_lastMouse.x;
            float dy = mouse.y - m_lastMouse.y;
            m_yaw -= dx * m_lookSensitivity;  // yaw
            m_pitch -= dy * m_lookSensitivity;  // pitch
            m_pitch  = std::clamp(m_pitch, -1.5708f, 1.5708f);
        }
        m_lastMouse = mouse;

        // WASD movement along camera's local axes (full 3D, respects pitch)
        float speed = m_moveSpeed * dt;
        Vec3 forward = {
            -std::sin(m_yaw) * std::cos(m_pitch),
            std::sin(m_pitch),
            -std::cos(m_yaw) * std::cos(m_pitch)
        };
        Vec3 right = { std::cos(m_yaw), 0.0f, -std::sin(m_yaw) };
        Vec3 position = m_camera.GetPosition();

        if (Input::IsKeyPressed(KeyCode::W))
            position = position + forward * speed;
        if (Input::IsKeyPressed(KeyCode::S))
            position = position - forward * speed;
        if (Input::IsKeyPressed(KeyCode::A))
            position = position - right * speed;
        if (Input::IsKeyPressed(KeyCode::D))
            position = position + right * speed;
        if (Input::IsKeyPressed(KeyCode::E))
            position.y += speed;
        if (Input::IsKeyPressed(KeyCode::Q))
            position.y -= speed;

        m_camera.SetView(position, m_yaw, m_pitch);
    }
}