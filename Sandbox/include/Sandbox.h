#pragma once

#include "AxiomEngine.h"

namespace axiom
{
    class Shader;
    class WindowResizeEvent;
    class KeyboardEvent;
}

class Sandbox : public axiom::Application
{
public:
    Sandbox(axiom::AppConfig appConfig);
    ~Sandbox() = default;
    void OnResize(const axiom::WindowResizeEvent& event);
    void OnKey(const axiom::KeyboardEvent& event);

protected:
    void OnRegisterModules() override;
    void OnApplicationRun() override;
    void OnRender() override;
    void OnUpdate() override;

private:
    axiom::SharedPtr<axiom::VertexBuffer> m_triangleVB;
    axiom::SharedPtr<axiom::IndexBuffer>  m_triangleIB;
    axiom::SharedPtr<axiom::VertexBuffer> m_rectangleVB;
    axiom::SharedPtr<axiom::IndexBuffer>  m_rectangleIB;
    axiom::SharedPtr<axiom::Shader>       m_shader;
    axiom::OrtographicCamera              m_camera;
};

