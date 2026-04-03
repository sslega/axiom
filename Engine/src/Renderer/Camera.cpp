#include "Renderer/Camera.h"
#include "Core/Assert.h"

namespace axiom
{
    axiom::OrtographicCamera::OrtographicCamera()
    : OrtographicCamera(-1.0f, 1.0f, -1.0f, 1.0f)
    {
    }

    OrtographicCamera::OrtographicCamera(float left, float right, float bottom, float top)
    : m_projectionMatrix(Matrix4::Ortho(left, right, bottom, top, -1.0f, 1.0f))
    , m_viewMatrix(Matrix4::Identity())
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
    }

    const Vec3 &OrtographicCamera::GetPosition() const
    {
        return m_position;
    }

    void OrtographicCamera::SetPosition(const Vec3 &position)
    {
        m_position = position;
        UpdateViewProjectionMatrix();
    }

    float OrtographicCamera::GetRotation() const
    {
        return m_rotation;
    }

    void OrtographicCamera::SetRotation(float rotation)
    {
        m_rotation = rotation;
        UpdateViewProjectionMatrix();
    }

    const Matrix4& OrtographicCamera::GetProjectionMatrix() const
    {
        return m_projectionMatrix;
    }

    const Matrix4& OrtographicCamera::GetViewMatrix() const
    {
        return m_viewMatrix;
    }

    const Matrix4& OrtographicCamera::GetViewProjectionMatrix() const
    {
        return m_viewProjectionMatrix;
    }

    void OrtographicCamera::UpdateViewProjectionMatrix()
    {
        Matrix4 translate = Matrix4::Translate(Vec3(-m_position.x, -m_position.y, -m_position.z));
        Matrix4 rotate    = Matrix4::Rotate(Vec3(0.0f, 0.0f, 1.0f), -m_rotation);
        m_viewMatrix             = rotate * translate;
        m_viewProjectionMatrix   = m_projectionMatrix * m_viewMatrix;
    }

}