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
    CameraComponent* m_cameraComponent;

    Vec3 m_lightColor;
    Vec3 m_lightDirection;
    float m_roughness = 0.5f;

    bool m_batchingEnabled = true;
    bool m_instancingEnabled = true;
    int m_debugDrawMode = 0;

    TransformComponent* m_cubeTransform;
};

