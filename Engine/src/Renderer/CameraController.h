#pragma once
#include "Scene/Component.h"
#include "Math/Vector.h"

namespace axiom
{
    class TransformComponent;

    class CameraController: public Component
    {
    public:
        float moveSpeed      = 0.5f;
        float lookSensitivity = 0.002f;
        
        virtual void OnUpdate(float deltaTime) override;

        virtual void Deserialize(Archive& ar) override;
        virtual void Serialize(Archive& ar) override;
        
    protected:
        TransformComponent* m_transformComponent;
        virtual void FileSubsystem() override;
    
    private:
        Vec2 m_lastMouse = {0, 0};
        bool m_firstUpdate = true;
    };
}