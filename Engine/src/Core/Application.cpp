#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"
#include "Renderer/RenderSubsystem.h"
#include "Resources/ResourceSubsystem.h"
#include "Resources/GLShaderLoader.h"
#include "Resources/Texture2DLoader.h"
#include "Resources/OBJLoader.h"
#include "Resources/MaterialLoader.h"
#include "Core/FileSubsystem.h"
#include "Scene/WorldSubsystem.h"
#include "ImGui/ImGuiSubsystem.h"
#include "Input/Input.h"
#include "Core/Log.h"
#include <imgui.h>
#include "Application.h"
#include "Global.h"

#include "Scene/TransformComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/CameraController.h"
#include "Renderer/MeshComponent.h"

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

        m_input = Input::Create(*m_applicationWindow);
        m_log = MakeUnique<ConsoleLog>();

        GApplication = this;
        GLog = m_log.get();
        GInput = m_input.get();
        GApplicationWindow = m_applicationWindow.get();
    }

    Application::~Application()
    {
        s_current = nullptr;
    }

    int Application::Run()
    {
        Log::Info("Starting Axiom Application...");

        RegisterComponentFactories();
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

        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->Update(deltaTime);
        }
        OnUpdate(deltaTime);
    }

    void Application::Render()
    {
        OnBeginFrame();

        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->BeginFrame();
        }
        
        OnRender();

        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->Render();
        }
        
        OnEndFrame();

        for (auto& id : m_subsystemOrder)
        {
            m_applicationSubsystems[id]->EndFrame();
        }
    }

    void Application::OnEndFrame()
    {
    }

    void Application::RegisterSubsystems()
    {
        fileSubsystem = RegisterSubsystem<FileSubsystem>();
        
        resourceSubsystem = RegisterSubsystem<ResourceSubsystem>();
        resourceSubsystem->RegisterLoader<GLShaderLoader>(".glsl");
        resourceSubsystem->RegisterLoader<Texture2DLoader>(".png");
        resourceSubsystem->RegisterLoader<Texture2DLoader>(".jpg");
        resourceSubsystem->RegisterLoader<OBJLoader>(".obj");

        worldSubsystem = RegisterSubsystem<WorldSubsystem>();
        renderSubsystem = RegisterSubsystem<RenderSubsystem>();
        resourceSubsystem->RegisterLoader<MaterialLoader>(".mat", *renderSubsystem);
        imGuiSubsystem = RegisterSubsystem<ImGuiSubsystem>();
    }

    void Application::InitializeSubsystems()
    {
        Log::Info("Initializing engine modules...");
        for (auto& id : m_subsystemOrder)
        {
            Log::Info("Initializing module: {}", id.name());
            m_applicationSubsystems[id]->Initialize();
        }
    }

    void Application::ShutdownSubsystems()
    {
        Log::Info("Shutting down engine modules...");
        for (auto it = m_subsystemOrder.rbegin(); it != m_subsystemOrder.rend(); ++it)
        {
            Log::Info("Shutting down module: {}", it->name());
            m_applicationSubsystems[*it]->Shutdown();
        }
    }

    void Application::RegisterComponentFactories()
    {
        ClassRegistry::Get().Register("TransformComponent", [] { return MakeUnique<TransformComponent>(); });
        ClassRegistry::Get().Register("CameraComponent", [] { return MakeUnique<CameraComponent>(); });
        ClassRegistry::Get().Register("CameraController", [] { return MakeUnique<CameraController>(); });
        ClassRegistry::Get().Register("MeshComponent", [] { return MakeUnique<MeshComponent>(); });
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