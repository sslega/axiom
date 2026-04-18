#include "Sandbox.h"
#include "EntryPoint.h"
#include "AxiomEngine.h"

#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"
#include "Renderer/Texture.h"
#include "Renderer/CameraComponent.h"
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
    FileSystemModule* fileSystemModule = GetModule<FileSystemModule>();
    fileSystemModule->Mount("Engine", AX_ENGINE_DIR);
    fileSystemModule->Mount("Project", AX_PROJECT_DIR);
}

void Sandbox::OnApplicationRun()
{
    RenderModule* Render = GetModule<RenderModule>();
    GraphicsDevice& Device = Render->GetGraphicsDevice();
    ResourceModule* Resource = GetModule<ResourceModule>();
    FileSystemModule* FileSystem = GetModule<FileSystemModule>();
    
    Scene* scene = GetModule<SceneModule>()->GetActiveScene();
    auto quadMesh = MakeShared<Quad>();
    // auto shader = Render->GetShader("engine://Shaders/Texture.glsl");
    auto shader = Render->GetShader("engine://Shaders/VertexColor.glsl");

    auto* cameraEntity = scene->CreateEntity("MainCamera");
    float ratio = GetApplicationWindow().AspectRatio();
    m_cameraComponent = cameraEntity->CreateComponent<CameraComponent>(-2.0f, 2.0f, -2.0f / ratio, 2.0f / ratio);

    auto textureResource = Resource->Load<Texture2DResource>("engine://Textures/heresy.png");
    m_texture = Device.CreateTexture2D(*textureResource);

    for (int y = -2; y <= 2; y++)
    {
        for (int x = -2; x <= 2; x++)
        {
            Entity* e = scene->CreateEntity();

            auto* t = e->CreateComponent<TransformComponent>();
            t->position = {x * 0.5f, y * 0.5f, 0.0f};
            t->scale    = {0.1f, 0.1f, 0.1f};

            auto mat = MakeShared<Material>();
            mat->m_shader = shader;
            mat->SetUniform("u_Color", Vec4{(x+2.f)/5.f, (y+2.f)/5.f, 0.f, 1.f});
            mat->SetUniform("u_Texture", 0);

            auto* m = e->CreateComponent<MeshComponent>();
            m->SetMesh(quadMesh);
            m->SetMaterial(mat);
        }
    }

    // Entity* entity = scene->CreateEntity("MyEntity");
    // TransformComponent* transform = entity->CreateComponent<TransformComponent>();
    // transform->position = {0, 0, 0};
    // transform->scale = {0.1f, 0.1f, 0.1f};


    /*
    float ratio = GetApplicationWindow().AspectRatio();
    m_camera = OrtographicCamera(-2.0f, 2.0f, -2.0f / ratio, 2.0f / ratio);

    Triangle triangle = Triangle();
    m_triangleVB = Device.CreateVertexBuffer(triangle);
    m_triangleIB = Device.CreateIndexBuffer(triangle);

    Quad quad = Quad();
    m_rectangleVB = Device.CreateVertexBuffer(quad);
    m_rectangleIB = Device.CreateIndexBuffer(quad);
    
    m_shader = Render->GetShader("engine://Shaders/Texture.glsl");

    auto textureResource = Resource->Load<Texture2DResource>("engine://Textures/heresy.png");
    m_texture = Device.CreateTexture2D(*textureResource);

    m_shader->UploadUniformInt("u_Texture", 0);
    */

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    printf("Window width: %d, window height: %d\n", event.m_newWidth, event.m_newHeight);
}

void Sandbox::OnRender()
{
    RenderModule* renderModule = GetModule<RenderModule>();
    // renderModule->BeginScene(m_cameraComponent->m_camera);

    /*
    float scale = 0.1f;
    Matrix4 scaleMatrix = Matrix4::Scale(Vec3(scale, scale, scale));
    for(int y = -2; y <= 2; y++)
    {
        for(int x = -2; x <= 2; x++)
        {
            Vec3 position = Vec3(x * 0.5, y * 0.5, 0.0f);
            Matrix4 transform = Matrix4::Translate(position) * scaleMatrix;
            Vec4 color = Vec4((x + 2.0f) / 5.0f, (y + 2.0f) / 5.0f, 0, 1.0f);
            m_shader->UploadUniform("u_Color", color);
            renderModule->Submit(m_rectangleVB, m_rectangleIB, m_shader, transform);
        }
    }

    Matrix4 triangleTransform = Matrix4::Translate(m_trianglePosition);
    m_texture->Bind();
    renderModule->Submit(m_rectangleVB, m_rectangleIB, m_shader, triangleTransform);

    // Matrix4 triangleTransform = Matrix4::Translate(m_trianglePosition);
    // m_shader->UploadUniformVec4("u_Color", m_triangleColor);
    // renderModule->Submit(m_triangleVB,  m_triangleIB,  m_shader, triangleTransform);
    */
    
    // renderModule->EndScene();

    ImGui::Begin("Hello World");
    ImGui::Text("Welcome to Yet Another Game Engine!");
    // ImGui::ColorEdit4("Triangle Color", &m_triangleColor.x);
    ImGui::End();
}

void Sandbox::OnUpdate(axiom::Timestep delta)
{
    

    if(Input::IsKeyPressed(KeyCode::LeftShift))
    {
        float translationSpeed = 1.0f * delta; // degrees per second
        if(Input::IsKeyPressed(KeyCode::Left))
        {
            m_trianglePosition.x -= translationSpeed;
        }

        if(Input::IsKeyPressed(KeyCode::Right))
        {
            m_trianglePosition.x += translationSpeed;
        }
        if(Input::IsKeyPressed(KeyCode::Up))
        {
            m_trianglePosition.y += translationSpeed;
        }
        if(Input::IsKeyPressed(KeyCode::Down))
        {
            m_trianglePosition.y -= translationSpeed;
        }
    }
    else
    {
        float rotationSpeed = 1.0f * delta; // degrees per second
        if(Input::IsKeyPressed(KeyCode::Left))
        {
            m_cameraComponent->m_camera.SetRotation(m_cameraComponent->m_camera.GetRotation() - rotationSpeed);
        }

        if(Input::IsKeyPressed(KeyCode::Right))
        {
            m_cameraComponent->m_camera.SetRotation(m_cameraComponent->m_camera.GetRotation() + rotationSpeed);
        }
    }
    

    
}
