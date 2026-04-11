#include "Core/Application.h"
#include "Platform/ApplicationWindow.h"
#include "Rendering/RenderModule.h"
#include "Resources/ResourceModule.h"
#include "Resources/GLShaderLoader.h"
#include "Core/FileSystemModule.h"
#include "Scene/SceneModule.h"
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

        for (auto& [type, module] : m_engineModules)
        {
            module->Update();
        }
        OnUpdate(ts);
    }

    void Application::Render()
    {
        m_applicationWindow->BeginFrame();

        for (auto& [type, module] : m_engineModules)
        {
            module->Render();
        }

        OnRender();
        OnImGuiRender();

        m_applicationWindow->EndFrame();
    }

    void Application::RegisterModules()
    {
        RegisterModule<FileSystemModule>();
        RegisterModule<RenderModule>();
        // RegisterModule<SceneModule>();

        ResourceModule* resourceModule = RegisterModule<ResourceModule>();
        resourceModule->RegisterLoader(".glsl", MakeUnique<GLShaderLoader>());
    }

    void Application::InitializeModules()
    {
        Log::Info("Initializing engine modules...");
        for (auto& [type, module] : m_engineModules)
        {
            Log::Info("Initializing module: {}", type.name());
            module->Initialize();
        }
        m_applicationWindow->OnModulesInitialized();
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