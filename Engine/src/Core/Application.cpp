#include "Axiom/Core/Application.h"
#include "Axiom/Platform/ApplicationWindow.h"
#include "Axiom/Rendering/RenderModule.h"

namespace axiom
{

    Application::Application(AppConfig appConfig)
    : m_appConfig(appConfig)
    {
        m_appConfig = appConfig;
        m_applicationWindow = IApplicationWindow::Create(m_appConfig.windowConfig);
    }

    int Application::Run()
    {
        printf("Starting Axiom Application...\n");
        
        RegisterModules();
        InitializeModules();
        OnApplicationStart();
        
        while (!m_applicationWindow->ShouldClose())
        {
            PoolEvents();
            Update();
            Render(); 
        }

        m_applicationWindow->CloseWindow();

        return 0;    
    }

    void Application::OnApplicationStart()
    {
    }

    void Application::PoolEvents()
    {
        m_applicationWindow->PoolEvents();
    }

    void Application::Update()
    {
        m_applicationWindow->Update();

        for (auto& [type, module] : m_engineModules)
        {
            module->Update();
        }
        
    }

    void Application::Render()
    {
        m_applicationWindow->Render();

        for (auto& [type, module] : m_engineModules)
        {
            module->Render();
        }
    }

    void Application::RegisterModules()
    {
        RegisterModule<RenderModule>();
    }

    void Application::InitializeModules()
    {
        printf("Initializing engine modules...\n");
        for (auto& [type, module] : m_engineModules)
        {
            printf("Initializing module: %s\n", type.name());
            module->Initialize();
        }
    }

    template <typename T>
    void Application::RegisterModule()
    {
        printf("Registering module: %s\n", typeid(T).name());
        m_engineModules[typeid(T)] = MakeUnique<T>(*this);
    }

    template <typename T>
    void Application::UnregisterModule()
    {
        m_engineModules.erase(typeid(T));
    }

    const RenderAPI Application::GetRenderAPI() const
    {
        return m_appConfig.renderAPI;
    }

    const IApplicationWindow& Application::GetApplicationWindow() const
    {
        return *m_applicationWindow;
    }
}