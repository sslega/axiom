#include "CameraController.h"
#include "Scene/Entity.h"
#include "Scene/TransformComponent.h"
#include "Input/Input.h"
#include "Math/Math.h"

namespace axiom
{
    void CameraController::OnUpdate(float deltaTime)
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
            m_transformComponent->rotation.y -= dx * lookSensitivity;  // yaw
            m_transformComponent->rotation.x -= dy * lookSensitivity;  // pitch
            m_transformComponent->rotation.x  = std::clamp(m_transformComponent->rotation.x, -1.5708f, 1.5708f);
        }
        m_lastMouse = mouse;

        // WASD movement along camera's local axes (full 3D, respects pitch)
        float speed = moveSpeed * dt;
        float yaw   = m_transformComponent->rotation.y;
        float pitch = m_transformComponent->rotation.x;
        Vec3 forward = {
            -std::sin(yaw) * std::cos(pitch),
            std::sin(pitch),
            -std::cos(yaw) * std::cos(pitch)
        };
        Vec3 right = { std::cos(yaw), 0.0f, -std::sin(yaw) };

        if (Input::IsKeyPressed(KeyCode::W))
            m_transformComponent->position = m_transformComponent->position + forward * speed;
        if (Input::IsKeyPressed(KeyCode::S))
            m_transformComponent->position = m_transformComponent->position - forward * speed;
        if (Input::IsKeyPressed(KeyCode::A))
            m_transformComponent->position = m_transformComponent->position - right * speed;
        if (Input::IsKeyPressed(KeyCode::D))
            m_transformComponent->position = m_transformComponent->position + right * speed;
        if (Input::IsKeyPressed(KeyCode::E))
            m_transformComponent->position.y += speed;
        if (Input::IsKeyPressed(KeyCode::Q))
            m_transformComponent->position.y -= speed;
    }
    
    void CameraController::OnInitialize()
    {
        m_transformComponent = GetEntity().GetComponent<TransformComponent>();
        AX_ASSERT(m_transformComponent, "TransformComponent required!");
    }
}