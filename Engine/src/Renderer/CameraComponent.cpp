#include "CameraComponent.h"
#include "Serialization/Archive.h"
#include "Math/Math.h"

namespace axiom
{
     

    template<>
    struct Serializer<Camera::ProjectionType>
    {
        static void Write(Archive& ar, const String& key, const Camera::ProjectionType& value)
        {
            ar.m_node[key] = (value == Camera::ProjectionType::Perspective) ? "perspective" : "orthographic";
        }
        static void Read(Archive& ar, const String& key, Camera::ProjectionType& value)
        {
            String s = ar.m_node[key].get<String>();
            value = (s == "orthographic") ? Camera::ProjectionType::Orthographic : Camera::ProjectionType::Perspective;
        }
    };

    CameraComponent::CameraComponent()
    : CameraComponent(ToRadians(60.0f), 16.0f/9.0f, 0.1f, 10000.0f)
    {
    }

    CameraComponent::CameraComponent(float fovYRadians, float aspectRatio, float near, float far)
        : m_camera(fovYRadians, aspectRatio, near, far)
    {
    }

    CameraComponent::CameraComponent(float left, float right, float bottom, float top, float near, float far)
    : m_camera(left, right, bottom, top, near, far)
    {
    }

    void CameraComponent::Serialize(Archive &ar)
    {
        float fov = ToDegrees(m_camera.GetFoV());
        float near = m_camera.GetNear();
        float far = m_camera.GetFar();
        Camera::ProjectionType projectionType = m_camera.GetProjectionType();
        float orthoSize = m_camera.GetOrthoSize();
        float orthoLeft = m_camera.GetOrthoLeft();
        float orthoRight = m_camera.GetOrthoRight();
        float orthoNear = m_camera.GetOrthoNear();
        float orthoFar = m_camera.GetOrthoFar();

        ar.Write("fov", fov);
        ar.Write("near", near);
        ar.Write("far", far);
        ar.Write("projectionType", projectionType);
        ar.Write("orthoSize", orthoSize);
        ar.Write("orthoLeft", orthoLeft);
        ar.Write("orthoRight", orthoRight);
        ar.Write("orthoNear", orthoNear);
        ar.Write("orthoFar", orthoFar);
    }

    void CameraComponent::Deserialize(Archive &ar)
    {
        float fov, near, far, orthoSize, orthoLeft, orthoRight, orthoNear, orthoFar, orthoBottom, orthoTop;
        Camera::ProjectionType projectionType;

        ar.Read("fov", fov);
        ar.Read("near", near);
        ar.Read("far", far);
        ar.Read("projectionType", projectionType );
        ar.Read("orthoSize", orthoSize);
        ar.Read("orthoLeft", orthoLeft);
        ar.Read("orthoRight", orthoRight);
        ar.Read("orthoNear", orthoNear);
        ar.Read("orthoFar", orthoFar);
        ar.Read("orthoBottom", orthoBottom);
        ar.Read("orthoTop", orthoTop);

        if(projectionType == Camera::ProjectionType::Orthographic)
        {
            m_camera = Camera(orthoLeft, orthoRight, orthoBottom, orthoTop, near, far);
        }
        else
        {
            m_camera = Camera(ToRadians(fov), 16.0f / 9.0f, near, far);
        }
    }
}