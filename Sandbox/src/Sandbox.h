#pragma once

#include "AxiomEngine.h"

using namespace axiom;

namespace axiom
{
    class Shader;
    class WindowResizeEvent;
    class KeyboardEvent;
    class Texture2D;
    class CameraComponent;
}

class Sandbox : public Application
{
public:
    Sandbox(AppConfig appConfig);
    ~Sandbox() = default;
    void OnResize(const WindowResizeEvent& event);

protected:
    void OnRegisterModules() override;
    void OnApplicationRun() override;
    void OnRender() override;
    void OnUpdate(float deltaTime) override;

private:
    SharedPtr<VertexBuffer> m_triangleVB;
    SharedPtr<IndexBuffer>  m_triangleIB;
    SharedPtr<VertexBuffer> m_rectangleVB;
    SharedPtr<IndexBuffer>  m_rectangleIB;
    SharedPtr<Shader>       m_shader;
    CameraComponent*        m_cameraComponent;
    SharedPtr<Texture2D>    m_texture;

    Vec3 m_trianglePosition;
    Vec4 m_triangleColor;
};

