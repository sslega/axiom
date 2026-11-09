#include "Application.h"
#include "Core/FileSubsystem.h"
#include "Reflection/EngineReflection.h"
#include "Reflection/ReflectionSubsystem.h"
#include "Core/Log.h"

#include "Platform/ApplicationWindow.h"

#include "Renderer/RenderSubsystem.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/CameraController.h"
#include "Renderer/MeshComponent.h"
#include "Renderer/LightComponent.h"

#include "Resources/ResourceSubsystem.h"
#include "Resources/GLShaderLoader.h"
#include "Resources/Texture2DLoader.h"
#include "Resources/OBJLoader.h"
#include "Resources/MaterialLoader.h"

#include "Scene/TransformComponent.h"
#include "Scene/WorldSubsystem.h"

#include "ImGui/ImGuiSubsystem.h"
#include "Input/Input.h"

namespace axiom
{
    Application* Application::s_current = nullptr; 

    Application::Application(AppConfig appConfig)
    : m_appConfig(appConfig)
    {
        if(!s_current)
        {
            s_current = this;
        }
        else
        {
            AX_ASSERT(false, "Application instance already exists!");
        }

        m_appConfig = appConfig;
        m_applicationWindow = ApplicationWindow::Create(m_appConfig.windowConfig);

        m_lastUpdateTime = std::chrono::steady_clock::now();

        m_input = IInput::Create(*m_applicationWindow);
        m_log = MakeUnique<ConsoleLog>();

        Log::SetInstance(m_log.get());
        Input::SetInstance(m_input.get());
    }

    Application::~Application()
    {
        Input::SetInstance(nullptr);
        Log::SetInstance(nullptr);
        s_current = nullptr;

        // Members die before bases, and the subsystems' GPU objects must go before the window does.
        DestroySubsystems();
    }

    int Application::Run()
    {
        Log::Info("Starting Axiom Application...");

        RegisterSubsystems();
        OnRegisterModules();
        InitializeSubsystems();
        OnInitializeModules();

        OnApplicationRun();

        while (!m_applicationWindow->ShouldClose())
        {
            PoolEvents();
            Update();
            Render();
        }

        ShutdownSubsystems();
        m_applicationWindow->CloseWindow();

        return 0;
    }

    void Application::PoolEvents()
    {
        m_applicationWindow->PollEvents();
    }

    void Application::Update()
    {
        auto now = std::chrono::steady_clock::now();
        Timestep ts = std::chrono::duration<float>(now - m_lastUpdateTime).count();
        float deltaTime = ts.Milliseconds();
        m_lastUpdateTime = now;

        m_applicationWindow->Update();

        UpdateSubsystems(deltaTime);
        OnUpdate(deltaTime);
    }

    void Application::Render()
    {
        OnBeginFrame();

        BeginFrameSubsystems();

        OnRender();

        RenderSubsystems();

        OnEndFrame();

        EndFrameSubsystems();
    }

    void Application::OnEndFrame()
    {
    }

    void Application::RegisterSubsystems()
    {
        fileSubsystem = RegisterSubsystem<FileSubsystem>();
        fileSubsystem->Mount(MountPoints::Engine, AX_ENGINE_DIR);

        reflectionSubsystem = RegisterSubsystem<ReflectionSubsystem>();
        reflectionSubsystem->AddRegistrar(&EngineReflection::Register);

        resourceSubsystem = RegisterSubsystem<ResourceSubsystem>();
        resourceSubsystem->RegisterLoader<GLShaderLoader>(".glsl");
        resourceSubsystem->RegisterLoader<Texture2DLoader>(".png");
        resourceSubsystem->RegisterLoader<Texture2DLoader>(".jpg");
        resourceSubsystem->RegisterLoader<OBJLoader>(".obj");

        worldSubsystem = RegisterSubsystem<WorldSubsystem>();
        renderSubsystem = RegisterSubsystem<RenderSubsystem>(*m_applicationWindow, m_appConfig.renderAPI);
        resourceSubsystem->RegisterLoader<MaterialLoader>(".mat", *renderSubsystem);
        imGuiSubsystem = RegisterSubsystem<ImGuiSubsystem>(*m_applicationWindow);

        
    }

    const GraphicsDevice::API Application::GetRenderAPI() const
    {
        return m_appConfig.renderAPI;
    }

    ApplicationWindow& Application::GetApplicationWindow()
    {
        return *m_applicationWindow;
    }
}