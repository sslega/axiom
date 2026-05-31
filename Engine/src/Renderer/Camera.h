#pragma once

#include "Math/Matrix4.h"


namespace axiom
{
    class Camera
    {
    public:
        enum class ProjectionType { Perspective, Orthographic };
        
        Camera(float fovYRadians, float aspectRatio, float near, float far);
        Camera(float left, float right, float bottom, float top, float near, float far);

        void SetProjectionType(ProjectionType type);
        inline ProjectionType GetProjectionType() const { return m_projectionType; }

        void SetOrthographic(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f);
        void SetPerspective(float fovYRadians, float aspectRatio, float near = 0.1f, float far = 1000.0f);
        void SetAspectRatio(float aspectRatio);
        
        void SetFoV(float fovYRadians);
        inline float GetFoV() const { return m_fovY; };
        inline float GetNear() const { return m_perspNear; }
        inline float GetFar() const { return m_perspFar; }

        void SetOrthoSize(float halfHeight);
        inline float GetOrthoSize() const { return m_orthoSize; }
        inline float GetOrthoNear() const { return m_orthoNear; }
        inline float GetOrthoFar() const { return m_orthoFar; }
        inline float GetOrthoLeft() const { return m_orthoLeft; }
        inline float GetOrthoRight() const { return m_orthoRight; }
        inline float GetOrthoTop() const { return m_orthoTop; }
        inline float GetOrthoBottom() const { return m_orthoBottom; }
        
        inline const Matrix4& GetProjectionMatrix() const { return m_projectionMatrix; };

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