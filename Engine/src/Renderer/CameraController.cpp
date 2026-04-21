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
        TransformComponent* transform = GetEntity()->GetComponent<TransformComponent>();

        // Mouse look
        Vec2 mouse = Input::GetMousePosition();
        bool rmbPressed = Input::IsMouseButtonPressed(MouseCode::Right);
        Input::SetCursorLocked(rmbPressed);
        if(rmbPressed)
        {
            float dx = mouse.x - m_lastMouse.x;
            float dy = mouse.y - m_lastMouse.y;
            transform->rotation.y -= dx * lookSensitivity;  // yaw
            transform->rotation.x -= dy * lookSensitivity;  // pitch
            transform->rotation.x  = std::clamp(transform->rotation.x, -1.5708f, 1.5708f);
        }
        m_lastMouse = mouse;

        // WASD movement along camera's local axes
        float speed = moveSpeed * dt;
        float yaw = transform->rotation.y;
        Vec3 forward = { -std::sin(yaw), 0.0f, -std::cos(yaw) };
        Vec3 right = { std::cos(yaw), 0.0f, -std::sin(yaw) };

        if (Input::IsKeyPressed(KeyCode::W)) 
        { 
            transform->position.x += forward.x * speed; 
            transform->position.z += forward.z * speed; 
        }
        if (Input::IsKeyPressed(KeyCode::S)) 
        { 
            transform->position.x -= forward.x * speed;
            transform->position.z -= forward.z * speed; 
        }
        if (Input::IsKeyPressed(KeyCode::A))
        { 
            transform->position.x -= right.x * speed;
            transform->position.z -= right.z * speed; 
        }
        if (Input::IsKeyPressed(KeyCode::D))
        { 
            transform->position.x += right.x * speed;
            transform->position.z += right.z * speed;
        }
    }
}