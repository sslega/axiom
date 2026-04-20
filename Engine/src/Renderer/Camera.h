#pragma once

#include "Math/Matrix4.h"


namespace axiom
{
    class Camera
    {
        enum class ProjectionType { Perspective, Orthographic };

    public:
        Camera(float fovYRadians, float aspectRatio, float near, float far);
        Camera(float left, float right, float bottom, float top, float near, float far);

        void SetProjectionType(ProjectionType type);
        inline ProjectionType GetProjectionType() const { return m_projectionType; }

        void SetOrthographic(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);
        void SetPerspective(float fovYRadians, float aspectRatio, float near = 0.1f, float far = 1000.0f);
        void SetAspectRatio(float aspectRatio);
        void SetOrthoSize(float halfHeight);
        inline float GetOrthoSize() const { return m_orthoSize; }
        void SetFoV(float fovYRadians);
        inline float GetFoV() const { return m_fovY; };
        
        inline const Matrix4& GetProjectionMatrix() const { return m_projectionMatrix; };
        static Matrix4 GetViewMatrix(Vec3 position, Vec3 rotation);

    private:
        ProjectionType m_projectionType = ProjectionType::Perspective;

        float m_orthoLeft = -1, m_orthoRight = 1;
        float m_orthoBottom = -1, m_orthoTop = 1;
        float m_orthoNear = -1, m_orthoFar = 1;
        float m_orthoSize = 1.0f;

        float m_fovY        = 1.0472f;
        float m_aspectRatio = 16.0f / 9.0f;
        float m_perspNear   = 0.1f;
        float m_perspFar    = 1000.0f;

        Matrix4 m_projectionMatrix;

        void RecalculateProjection();
    };

}