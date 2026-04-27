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
    auto shader = renderModule->GetShader("engine://Shaders/Texture.glsl");
    auto litShader = renderModule->GetShader("engine://Shaders/BlinnPhong.glsl");
    auto vertexColorShader = renderModule->GetShader("engine://Shaders/VertexColor.glsl");

    auto& cameraEntity = scene.CreateEntity("MainCamera");
    auto& transformComponent = cameraEntity.CreateComponent<TransformComponent>();
    transformComponent.position = Vec3(0.0f, 0.0f, 2.0f);
    m_cameraTransform = &transformComponent;
    float aspectRatio = GetApplicationWindow().GetAspectRatio();
    m_cameraComponent = &cameraEntity.CreateComponent<CameraComponent>(ToRadians(60.0f), aspectRatio, 0.1f, 1000.0f);
    cameraEntity.CreateComponent<CameraController>();

    auto textureResource = resourceModule->Load<Texture2DResource>("engine://Textures/heresy.png");
    m_texture = renderModule->GetGraphicsDevice().CreateTexture2D(*textureResource);

    
    m_litMaterial = MakeShared<Material>(litShader);

    m_lightDirection = Vec3{ 0.75, 1.0, 0.5 };
    m_lightColor = Vec3{ 1.0f, 0.9f,  0.8f };
    m_roughness = 0.3f;

    m_litMaterial->SetUniform("u_LightDir",   m_lightDirection);
    m_litMaterial->SetUniform("u_LightColor", m_lightColor);
    m_litMaterial->SetUniform("u_Roughness", m_roughness);

    Entity& entity = scene.CreateEntity();

    auto& transform = entity.CreateComponent<TransformComponent>();
    transform.scale = {0.25f, 0.25f, 0.25f};

    auto& mesh = entity.CreateComponent<MeshComponent>();
    mesh.SetMesh(MakeShared<Cube>());
    mesh.SetMaterial(m_litMaterial);

    /*
    // Common, instanced texture material
    auto mat = MakeShared<Material>(shader);
    mat->SetTexture("u_Texture", m_texture, 0);

    for (int y = -2; y <= 2; y++)
    {
        for (int x = -2; x <= 2; x++)
        {
            Entity& e = scene.CreateEntity();

            auto& transform = e.CreateComponent<TransformComponent>();
            transform.position = {x * 0.5f, y * 0.5f, 0.0f};
            transform.scale    = {0.1f, 0.1f, 0.1f};

            auto& mesh = e.CreateComponent<MeshComponent>();
            // mesh.SetMesh(quadMesh);
            mesh.SetMesh(cubeMesh);
            // mesh.SetMaterial(mat);
            mesh.SetMaterial(m_litMaterial);
        }
    }

    for (int y = -2; y < 2; y++)
    {
        for (int x = -2; x < 2; x++)
        {
            Entity& e = scene.CreateEntity();

            auto& transform = e.CreateComponent<TransformComponent>();
            transform.position = {x * 0.5f + 0.25f, y * 0.5f + 0.25f, 0.0f};
            transform.scale    = {0.1f, 0.1f, 0.1f};

            auto& mesh = e.CreateComponent<MeshComponent>();
            auto triangle = MakeShared<Triangle>();
            mesh.SetMesh(triangle);
            mesh.SetMaterial(mat);
        }
    }
    
    */

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
}

void Sandbox::OnUpdate(float deltaTime)
{
    
}
