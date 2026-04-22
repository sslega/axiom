#pragma once

#include "Scene/Component.h"
#include "Renderer/Camera.h"

namespace axiom
{
    class CameraComponent : public Component
    {
    public:
        CameraComponent(float fovYRadians, float aspectRatio, float near, float far);
        CameraComponent(float left, float right, float bottom, float top, float near, float far);

        inline void SetOrthographic(float left, float right, float bottom, float top, float near = -1.0f, float far = 1.0f) { m_camera.SetOrthographic(left, right, bottom, top, near, far); }
        inline void SetPerspective(float fovYRadians, float aspectRatio, float near = 0.1f, float far = 1000.0f) { m_camera.SetPerspective(fovYRadians, aspectRatio, near, far); }
        inline void SetAspectRatio(float aspectRatio) { m_camera.SetAspectRatio(aspectRatio); }
        inline void SetOrthoSize(float halfHeight) { m_camera.SetOrthoSize(halfHeight); }
        inline float GetOrthoSize() const { return m_camera.GetOrthoSize(); }
        inline void SetFoV(float fovYRadians) { m_camera.SetFoV(fovYRadians); }
        inline float GetFoV() const { return m_camera.GetFoV(); }
        
        inline const Matrix4& GetProjectionMatrix() const { return m_camera.GetProjectionMatrix(); }
        static Matrix4 GetViewMatrix(Vec3 position, Vec3 rotation) { return Camera::GetViewMatrix(position, rotation); }
    
    protected:
        Camera m_camera;
    };
}