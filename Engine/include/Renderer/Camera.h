#pragma once

#include "Math/Matrix4.h"


namespace axiom
{
    class OrtographicCamera
    {
    public:
        OrtographicCamera();
        OrtographicCamera(float left, float right, float bottom, float top);
        
        const Vec3& GetPosition() const;
        void SetPosition(const Vec3& position);
        
        float GetRotation() const;
        void SetRotation(float rotation);

        const Matrix4& GetProjectionMatrix() const;
        const Matrix4& GetViewMatrix() const;
        const Matrix4& GetViewProjectionMatrix() const;

    private:
        Matrix4 m_projectionMatrix;
        Matrix4 m_viewMatrix;
        Matrix4 m_viewProjectionMatrix;

        Vec3 m_position;
        float m_rotation = 0.0f;

        void UpdateViewProjectionMatrix();
    };

}