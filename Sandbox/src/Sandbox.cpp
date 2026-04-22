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
    auto shader = renderModule->GetShader("engine://Shaders/Texture.glsl");
    // auto shader = Render->GetShader("engine://Shaders/VertexColor.glsl");

    auto& cameraEntity = scene.CreateEntity("MainCamera");
    auto& transformComponent = cameraEntity.CreateComponent<TransformComponent>();
    transformComponent.position = Vec3(0.0f, 0.0f, 2.0f);
    float aspectRatio = GetApplicationWindow().AspectRatio();
    m_cameraComponent = &cameraEntity.CreateComponent<CameraComponent>(ToRadians(60.0f), aspectRatio, 0.1f, 1000.0f);
    cameraEntity.CreateComponent<CameraController>();

    auto textureResource = resourceModule->Load<Texture2DResource>("engine://Textures/heresy.png");
    m_texture = renderModule->GetGraphicsDevice().CreateTexture2D(*textureResource);

    // Common, instanced material
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
            mesh.SetMesh(quadMesh);
            mesh.SetMaterial(mat);
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

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    float aspect = (float)event.m_newWidth / event.m_newHeight;
    m_cameraComponent->SetAspectRatio(aspect);
}

void Sandbox::OnRender()
{
    ImGui::Begin("Params");
    ImGui::ColorEdit4("Triangle Color", &m_triangleColor.x);
    ImGui::End();
}

void Sandbox::OnUpdate(float deltaTime)
{
    
}
