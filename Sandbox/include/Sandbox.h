#pragma once

#include "AxiomEngine.h"

using namespace axiom;

namespace axiom
{
    class Shader;
    class WindowResizeEvent;
    class KeyboardEvent;
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
    void OnUpdate(Timestep delta) override;
    void OnImGuiRender() override;

private:
    SharedPtr<VertexBuffer> m_triangleVB;
    SharedPtr<IndexBuffer>  m_triangleIB;
    SharedPtr<VertexBuffer> m_rectangleVB;
    SharedPtr<IndexBuffer>  m_rectangleIB;
    SharedPtr<Shader>       m_shader;
    OrtographicCamera       m_camera;

    Vec3 m_trianglePosition;
};

