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
    
    Scene& scene = sceneModule->GetActiveScene();
    auto quadMesh = MakeShared<Quad>();
    auto triangleMesh = MakeShared<Triangle>();
    auto cubeMesh = MakeShared<Cube>();
    auto shader = renderModule->GetShader("project://Shaders/Texture.glsl");
    auto litShader = renderModule->GetShader("project://Shaders/LightModels.glsl");
    auto vertexColorShader = renderModule->GetShader("project://Shaders/VertexColor.glsl");

    SharedPtr<MeshResource> bunny = resourceModule->Load<MeshResource>("project://Meshes/bunny.obj");

    auto& cameraEntity = scene.CreateEntity("MainCamera");
    {
        auto& transform = cameraEntity.CreateComponent<TransformComponent>();
        transform.position = Vec3(0.0f, 0.25f, 2.0f);
        m_cameraTransform = &transform;
        float aspectRatio = GetApplicationWindow().GetAspectRatio();
        m_cameraComponent = &cameraEntity.CreateComponent<CameraComponent>(ToRadians(60.0f), aspectRatio, 0.1f, 1000.0f);
        cameraEntity.CreateComponent<CameraController>();
    }
    

    auto textureResource = resourceModule->Load<Texture2DResource>("engine://Textures/heresy.png");
    m_texture = renderModule->GetGraphicsDevice().CreateTexture2D(*textureResource);

    m_lightDirection = Vec3{ 0.75, 1.0, 0.5 };
    m_lightColor = Vec3{ 1.0f, 0.9f,  0.8f };
    m_roughness = 0.3f;
    
    m_litMaterial = MakeShared<Material>(litShader);
    m_litMaterial->SetUniform("u_LightDir",   m_lightDirection);
    m_litMaterial->SetUniform("u_LightColor", m_lightColor);
    m_litMaterial->SetUniform("u_Roughness", m_roughness);

    // Entity& boxEntity = scene.CreateEntity();
    // {
    //     auto& transform = boxEntity.CreateComponent<TransformComponent>();
    //     transform.scale = 0.25f;
    //     transform.position = {0, 0.25f, 0};
    //     // transform.rotation = {0, ToRadians(45), 0};
    //     m_cubeTransform = &transform;

    //     auto& mesh = boxEntity.CreateComponent<MeshComponent>();
    //     mesh.SetMesh(MakeShared<Cube>());
    //     mesh.SetMaterial(m_litMaterial);
    // }

    Entity& bunnyEntity = scene.CreateEntity();
    {
        auto& transform = bunnyEntity.CreateComponent<TransformComponent>();
        transform.scale = 2.0f;
        m_cubeTransform = &transform;

        auto& mesh = bunnyEntity.CreateComponent<MeshComponent>();
        mesh.SetMesh(bunny);
        mesh.SetMaterial(m_litMaterial);
    }

    Entity& planeEntity = scene.CreateEntity();
    {
        auto& transform = planeEntity.CreateComponent<TransformComponent>();
        transform.scale = {5.0f, 5.0f, 5.0f};
        transform.rotation = {ToRadians(-90), 0, 0};

        auto& mesh = planeEntity.CreateComponent<MeshComponent>();
        mesh.SetMesh(MakeShared<Quad>());
        mesh.SetMaterial(m_litMaterial);
    }

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    float aspect = (float)event.m_newWidth / event.m_newHeight;
    m_cameraComponent->SetAspectRatio(aspect);
}

void Sandbox::OnRender()
{
    if (m_cameraTransform)
        m_litMaterial->SetUniform("u_CameraPos", m_cameraTransform->position);

    m_litMaterial->SetUniform("u_LightDir",   m_lightDirection);
    m_litMaterial->SetUniform("u_LightColor", m_lightColor);
    m_litMaterial->SetUniform("u_Roughness", m_roughness);

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
    m_cubeRotation++;
    m_cubeTransform->rotation.y = ToRadians(m_cubeRotation);
}
