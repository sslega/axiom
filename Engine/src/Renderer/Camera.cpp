#include "Renderer/Camera.h"
#include "Core/Assert.h"
#include "Camera.h"

namespace axiom
{
    //TODO: aspecrt artion should not be in the constructor
    Camera::Camera(float fovYRadians, float aspectRatio, float near, float far)
    {
        SetPerspective(fovYRadians, aspectRatio, near, far);
    }

    Camera::Camera(float left, float right, float bottom, float top, float near, float far)
    {
        SetOrthographic(left, right, bottom, top, near, far);
    }

    void Camera::SetProjectionType(ProjectionType type)
    {
        m_projectionType = type;
        RecalculateProjection();
    }

    void Camera::SetOrthographic(float left, float right, float bottom, float top, float near, float far)
    {
        m_projectionType = ProjectionType::Orthographic;
        m_orthoLeft = left; m_orthoRight = right;
        m_orthoBottom = bottom; m_orthoTop = top;
        m_orthoNear = near; m_orthoFar = far;
        m_orthoSize = (top - bottom) * 0.5f;
        RecalculateProjection();
    }

    void Camera::SetPerspective(float fovYRadians, float aspectRatio, float near, float far)
    {
        m_projectionType = ProjectionType::Perspective;
        m_fovY = fovYRadians;
        m_aspectRatio = aspectRatio;
        m_perspNear = near;
        m_perspFar = far;
        RecalculateProjection();
    }

    void Camera::SetOrthoSize(float halfHeight)
    {
        m_orthoSize = halfHeight;
        SetAspectRatio(m_aspectRatio); 
    }

    void Camera::SetFoV(float fovYRadians)
    {
        m_fovY = fovYRadians;
        RecalculateProjection();
    }

    void Camera::RecalculateProjection()
    {
        if (m_projectionType == ProjectionType::Orthographic)
        {
            m_projectionMatrix = Matrix4::Ortho(m_orthoLeft, m_orthoRight, m_orthoBottom, m_orthoTop, m_orthoNear, m_orthoFar);
        }
        else
        {
            m_projectionMatrix = Matrix4::Perspective(m_fovY, m_aspectRatio, m_perspNear, m_perspFar);
        }
    }


    void Camera::SetAspectRatio(float aspectRatio)
    {
        m_aspectRatio = aspectRatio;
        if (m_projectionType == ProjectionType::Orthographic)
        {
            m_orthoLeft   = -m_orthoSize * aspectRatio;
            m_orthoRight  =  m_orthoSize * aspectRatio;
            m_orthoBottom = -m_orthoSize;
            m_orthoTop    =  m_orthoSize;
        }
        RecalculateProjection();
    }
}