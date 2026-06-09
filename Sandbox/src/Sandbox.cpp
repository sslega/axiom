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
    windowConfig.width = 1280;
    windowConfig.height = 720;
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
    FileSubsystem& fileSystemModule = GetSubsystem<FileSubsystem>();
    fileSystemModule.Mount("Engine", AX_ENGINE_DIR);
    fileSystemModule.Mount("Project", AX_PROJECT_DIR);
}

void Sandbox::OnApplicationRun()
{
    worldSubsystem->LoadScene("project://Scenes/main.scene.json");

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    float aspect = (float)event.m_newWidth / event.m_newHeight;
    m_cameraComponent->SetAspectRatio(aspect);
}

void Sandbox::OnRender()
{
    const char* debugModes[]{"None","World Normal"};

    ImGui::Begin("Render Settings");
    ImGui::Checkbox("Enable batching", &m_batchingEnabled);
    ImGui::Checkbox("Enable instancing", &m_instancingEnabled);
    ImGui::Combo("DebugDraw mode", &m_debugDrawMode, debugModes, IM_ARRAYSIZE(debugModes));
    if(ImGui::Button("Reload Shaders"))
    {
        renderSubsystem->ReloadShaders();
    }
    ImGui::End();


    renderSubsystem->SetDebugDrawMode(m_debugDrawMode);
    renderSubsystem->SetBatchingEnabled(m_batchingEnabled);
    renderSubsystem->SetInstancingEnabled(m_instancingEnabled);
}

void Sandbox::OnUpdate(float deltaTime)
{
    // m_cubeRotation++;
    // m_cubeTransform->rotation.y = ToRadians(m_cubeRotation);
}
