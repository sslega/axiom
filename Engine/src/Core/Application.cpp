#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"
#include "Renderer/RenderModule.h"
#include "Resources/ResourceModule.h"
#include "Resources/GLShaderLoader.h"
#include "Resources/Texture2DLoader.h"
#include "Core/FileSystemModule.h"
#include "Scene/SceneModule.h"
#include "ImGui/ImGuiModule.h"
#include "Input/Input.h"
#include "Core/Log.h"

namespace axiom
{

    Application* Application::s_instance = nullptr;

    Application::Application(AppConfig appConfig)
    : m_appConfig(appConfig)
    {
        if(!s_instance)
        {
            s_instance = this;
        }
        else
        {
            AX_ASSERT(false, "Application instance already exists!");
        }

        // forces each printf to flush immediately.
        // TODO: Remove after adding proper logging
        setvbuf(stdout, NULL, _IONBF, 0);
        m_appConfig = appConfig;
        m_applicationWindow = ApplicationWindow::Create(m_appConfig.windowConfig);

        m_input = Input::Create(*m_applicationWindow);
        m_log = MakeUnique<ConsoleLog>();
        m_lastFrameTime = std::chrono::steady_clock::now();
    }

    Application::~Application()
    {
        s_instance = nullptr;
    }

    int Application::Run()
    {
        Log::Info("Starting Axiom Application...");

        RegisterModules();
        OnRegisterModules();
        InitializeModules();
        OnInitializeModules();

        OnApplicationRun();

        while (!m_applicationWindow->ShouldClose())
        {
            PoolEvents();
            Update();
            Render();
        }

        ShutdownModules();
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
        Timestep ts = std::chrono::duration<float>(now - m_lastFrameTime).count();
        m_lastFrameTime = now;

        m_applicationWindow->Update();

        for (auto& id : m_moduleOrder)
        {
            m_engineModules[id]->Update();
        }
        OnUpdate(ts);
    }

    void Application::Render()
    {
        for (auto& id : m_moduleOrder)
            m_engineModules[id]->BeginFrame();

        for (auto& id : m_moduleOrder)
            m_engineModules[id]->Render();

        OnRender();

        for (auto& id : m_moduleOrder)
            m_engineModules[id]->EndFrame();

        GetModule<RenderModule>()->GetGraphicsDevice().Present();
    }

    void Application::RegisterModules()
    {
        RegisterModule<FileSystemModule>();
        
        ResourceModule* resourceModule = RegisterModule<ResourceModule>();
        resourceModule->RegisterLoader(".glsl", MakeUnique<GLShaderLoader>());
        resourceModule->RegisterLoader(".png",  MakeUnique<Texture2DLoader>());
        resourceModule->RegisterLoader(".jpg",  MakeUnique<Texture2DLoader>());

        RegisterModule<RenderModule>();
        RegisterModule<ImGuiModule>();
    }

    void Application::InitializeModules()
    {
        Log::Info("Initializing engine modules...");
        for (auto& id : m_moduleOrder)
        {
            Log::Info("Initializing module: {}", id.name());
            m_engineModules[id]->Initialize();
        }
    }

    void Application::ShutdownModules()
    {
        Log::Info("Shutting down engine modules...");
        for (auto it = m_moduleOrder.rbegin(); it != m_moduleOrder.rend(); ++it)
        {
            Log::Info("Shutting down module: {}", it->name());
            m_engineModules[*it]->Shutdown();
        }
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
