#pragma once

#include "Core/Application.h"
#include "Renderer/Buffer.h"
#include "Renderer/Camera.h"
#include "Core/Types.h"

namespace axiom { class Shader; }

class Sandbox : public axiom::Application
{
public:
    Sandbox(axiom::AppConfig appConfig);
    ~Sandbox() = default;

protected:
    void OnRegisterModules() override;
    void OnApplicationRun() override;
    void OnRender() override;

private:
    axiom::SharedPtr<axiom::VertexBuffer> m_triangleVB;
    axiom::SharedPtr<axiom::IndexBuffer>  m_triangleIB;
    axiom::SharedPtr<axiom::VertexBuffer> m_rectangleVB;
    axiom::SharedPtr<axiom::IndexBuffer>  m_rectangleIB;
    axiom::SharedPtr<axiom::Shader>       m_shader;
    axiom::OrtographicCamera              m_camera;
};

