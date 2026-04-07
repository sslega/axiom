#include "EntryPoint.h"
#include "Core/Application.h"
#include "Core/FileSystemModule.h"
#include "Rendering/RenderModule.h"
#include "Renderer/GraphicsDevice.h"
#include "Renderer/Shader.h"

#include "Sandbox.h"

#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"

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
        -0.75f, -0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f,
         0.75f, -0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f,
         0.75f,  0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f,
        -0.75f,  0.75f, 0.0f, 0.5f, 0.5f, 0.75f, 1.0f
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

        out vec3 v_Position;
        out vec4 v_Color;

        void main()
        {
            v_Position = a_Position;
            v_Color = a_Color;
            gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
        }
    )";

    String fragmentSrc = R"(
        #version 330 core

        layout(location = 0) out vec4 color;

        in vec3 v_Position;
        in vec4 v_Color;

        void main()
        {
            color = v_Color;
        }
    )";

    m_shader = GFX.CreateShader(vertexSrc, fragmentSrc);

    GetApplicationWindow().AddEventListener(&Sandbox::OnResize, this);
    GetApplicationWindow().AddEventListener(&Sandbox::OnKey, this);
}

void Sandbox::OnResize(const WindowResizeEvent& event)
{
    printf("Window width: %d, window height: %d\n", event.m_newWidth, event.m_newHeight);
}

void Sandbox::OnKey(const KeyboardEvent& event)
{
    printf("Key %s : %d\n", event.m_action == KeyboardEvent::Action::RELEASE ? "released" : "pressed", event.m_keyCode);
}

void Sandbox::OnRender()
{
    RenderModule* renderModule = GetModule<RenderModule>();
    renderModule->BeginScene(m_camera);
    renderModule->Submit(m_rectangleVB, m_rectangleIB, m_shader);
    renderModule->Submit(m_triangleVB,  m_triangleIB,  m_shader);
    renderModule->EndScene();
}


