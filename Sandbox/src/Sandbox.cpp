#include "Sandbox.h"
#include "EntryPoint.h"
#include "AxiomEngine.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

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
    RenderModule* renderModule = GetModule<RenderModule>();
    GraphicsDevice& GFX = renderModule->GetGraphicsDevice();

    float ratio = GetApplicationWindow().AspectRatio();
    m_camera = OrtographicCamera(-2.0f, 2.0f, -2.0f / ratio, 2.0f / ratio);

    uint32 triangleIndices[3] = {0, 1, 2};
    float triangleVertices[7 * 3] =
    {
        -0.5f, -0.5f, 0.0f, 1.0f, 0.5f, 0.5f, 1.0f,
         0.5f, -0.5f, 0.0f, 0.5f, 1.0f, 0.5f, 1.0f,
         0.0f,  0.5f, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f
    };

    uint32 squareIndices[6] = {0, 1, 2, 2, 3, 0};
    float squareVertices[7 * 4] =
    {
        -0.15f, -0.15f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.15f, -0.15f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
         0.15f,  0.15f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        -0.15f,  0.15f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f
    };

    BufferLayout layout = {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float4, "a_Color"}
    };

    m_triangleVB = GFX.CreateVertexBuffer(triangleVertices, sizeof(triangleVertices));
    m_triangleVB->SetLayout(layout);
    m_triangleIB = GFX.CreateIndexBuffer(triangleIndices, 3);

    m_rectangleVB = GFX.CreateVertexBuffer(squareVertices, sizeof(squareVertices));
    m_rectangleVB->SetLayout(layout);
    m_rectangleIB = GFX.CreateIndexBuffer(squareIndices, 6);

    String vertexSrc = R"(
        #version 330 core

        layout(location = 0) in vec3 a_Position;
        layout(location = 1) in vec4 a_Color;

        uniform mat4 u_ViewProjection;
        uniform mat4 u_Transform;

        out vec3 v_Position;
        out vec4 v_Color;

        void main()
        {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
        }
    )";

    String fragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        in vec4 v_Color;

        uniform vec4 u_Color;

        void main()
        {
            color = v_Color * u_Color;
        }
    )";

    m_shader = GFX.CreateShader(vertexSrc, fragmentSrc);

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    printf("Window width: %d, window height: %d\n", event.m_newWidth, event.m_newHeight);
}

void Sandbox::OnRender()
{
    RenderModule* renderModule = GetModule<RenderModule>();
    renderModule->BeginScene(m_camera);
    for(int y = -2; y <= 2; y++)
    {
        for(int x = -2; x <= 2; x++)
        {
            Vec3 position = Vec3(x * 0.5, y * 0.5, 0.0f);
            Matrix4 transform = Matrix4::Translate(position);
            Vec4 color = Vec4((x + 2.0f) / 5.0f, (y + 2.0f) / 5.0f, 0, 1.0f);
            m_shader->UploadUniformVec4("u_Color", color);
            renderModule->Submit(m_rectangleVB, m_rectangleIB, m_shader, transform);
        }
    }

    Matrix4 triangleTransform = Matrix4::Translate(m_trianglePosition);
    renderModule->Submit(m_triangleVB,  m_triangleIB,  m_shader, triangleTransform);
    
    renderModule->EndScene();
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
            m_camera.SetRotation(m_camera.GetRotation() - rotationSpeed);
        }

        if(Input::IsKeyPressed(KeyCode::Right))
        {
            m_camera.SetRotation(m_camera.GetRotation() + rotationSpeed);
        }
    }
    

    
}
