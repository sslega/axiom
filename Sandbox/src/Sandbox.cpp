#include "Sandbox.h"
#include "EntryPoint.h"
#include "AxiomEngine.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/CameraController.h"
#include "Resources/ShaderResource.h"
#include "Resources/Texture2DResource.h"
#include "Geometry/Quad.h"
#include "Geometry/Triangle.h"
#include "Geometry/Cube.h"

using namespace axiom;


UniquePtr<Application> CreateApplication()
{

    AppConfig appConfig;
    appConfig.renderAPI = GraphicsDevice::API::OpenGL;
    
    AppWindowConfig windowConfig;
    windowConfig.width = 800;
    windowConfig.height = 600;
    windowConfig.title = "Yet Another Game Engine";
    windowConfig.vsync = true;
    windowConfig.fullscreen = false;
    windowConfig.backend = ApplicationWindowBackend::GLFW;

    appConfig.windowConfig = windowConfig;
    return MakeUnique<Sandbox>(appConfig);
};

Sandbox::Sandbox(AppConfig appConfig)
: Application(appConfig)
{
}

void Sandbox::OnRegisterModules()
{
    FileSystemModule& fileSystemModule = GetModule<FileSystemModule>();
    fileSystemModule.Mount("Engine", AX_ENGINE_DIR);
    fileSystemModule.Mount("Project", AX_PROJECT_DIR);
}

void Sandbox::OnApplicationRun()
{
    sceneModule->LoadScene("project://Scenes/main.scene.json");

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    float aspect = (float)event.m_newWidth / event.m_newHeight;
    m_cameraComponent->SetAspectRatio(aspect);
}

void Sandbox::OnRender()
{
    // if (m_cameraTransform)
    //     m_litMaterial->SetUniform("u_CameraPos", m_cameraTransform->position);

    // m_litMaterial->SetUniform("u_LightDir",   m_lightDirection);
    // m_litMaterial->SetUniform("u_LightColor", m_lightColor);
    // m_litMaterial->SetUniform("u_Roughness", m_roughness);

    ImGui::Begin("Material Params");
    ImGui::ColorEdit3("Light Color", &m_lightColor.x);
    ImGui::SliderFloat3("Light Direction", &m_lightDirection.x, 0, 1);
    ImGui::SliderFloat("Roughness", &m_roughness, 0, 1);
    ImGui::End();

    const char* debugModes[]{"None","World Normal"};

    ImGui::Begin("Render Settings");
    ImGui::Checkbox("Enable batching", &m_batchingEnabled);
    ImGui::Checkbox("Enable instancing", &m_instancingEnabled);
    ImGui::Combo("DebugDraw mode", &m_debugDrawMode, debugModes, IM_ARRAYSIZE(debugModes));
    ImGui::End();


    renderModule->SetDebugDrawMode(m_debugDrawMode);
    renderModule->SetBatchingEnabled(m_batchingEnabled);
    renderModule->SetInstancingEnabled(m_instancingEnabled);
}

void Sandbox::OnUpdate(float deltaTime)
{
    // m_cubeRotation++;
    // m_cubeTransform->rotation.y = ToRadians(m_cubeRotation);
}
