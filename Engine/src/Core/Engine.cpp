#include "axiom/Core/Engine.h"
#include "axiom/Core/Application.h"
#include "axiom/Platform/ApplicationWindow.h"
#include "axiom/Renderer/RenderModule.h"

namespace axiom
{

    Engine::Engine(EngineConfig config, ApplicationWindowConfig windowConfig)
    {
        m_engineConfig = config;
        m_applicationWindow = IApplicationWindow::Create(windowConfig);
    }

    int Engine::Run(Application* game)
    {
        printf("Starting Axiom Engine...\n");
        
        RegisterModules();
        InitializeModules();
        
        while (!m_applicationWindow->ShouldClose())
        {
            PoolEvents();
            Update();
            Render(); 
        }

        m_applicationWindow->CloseWindow();

        return 0;    
    }

    void Engine::PoolEvents()
    {
        m_applicationWindow->PoolEvents();
    }

    void Engine::Update()
    {
        m_applicationWindow->Update();

        for (auto& [type, module] : m_engineModules)
        {
            module->Update();
        }
        
    }

    void Engine::Render()
    {
        m_applicationWindow->Render();

        for (auto& [type, module] : m_engineModules)
        {
            module->Render();
        }
    }

    void Engine::RegisterModules()
    {
        RegisterModule<RenderModule>();
    }

    void Engine::InitializeModules()
    {
        printf("Initializing engine modules...\n");
        for (auto& [type, module] : m_engineModules)
        {
            printf("Initializing module: %s\n", type.name());
            module->Initialize();
        }
    }

    template <typename T>
    void Engine::RegisterModule()
    {
        printf("Registering module: %s\n", typeid(T).name());
        m_engineModules[typeid(T)] = MakeUnique<T>(*this);
    }

    template <typename T>
    void Engine::UnregisterModule()
    {
        m_engineModules.erase(typeid(T));
    }

    const RenderAPI Engine::GetRenderAPI() const
    {
        return m_engineConfig.renderAPI;
    }

    const IApplicationWindow& Engine::GetApplicationWindow() const
    {
        return *m_applicationWindow;
    }
}