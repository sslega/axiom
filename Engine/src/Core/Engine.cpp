#include "axiom/Core/Engine.h"
#include "axiom/Core/Game.h"
#include "axiom/Platform/ApplicationWindow.h"

namespace axiom
{

    Engine::Engine(EngineConfig config, ApplicationWindowDesc windowConfig)
    {
        m_engineConfig = config;
        m_applicationWindow = IApplicationWindow::Create(windowConfig);
        // m_applicationWindow = std::unique_ptr<IApplicationWindow>(&window);
    }

    int Engine::Run(Game *game)
    {
        std::printf("Starting Axiom Engine...\n");
        
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

    void Engine::InitializeModules()
    {
        printf("Initializing engine modules...\n");
        RegisterModule<Renderer>();
    }

    template <typename T>
    void Engine::RegisterModule()
    {
        std::printf("Registering module: %s\n", typeid(T).name());
        m_engineModules[typeid(T)] = std::make_unique<T>();
    }

    template <typename T>
    void Engine::UnregisterModule()
    {
        m_engineModules.erase(typeid(T));
    }
}