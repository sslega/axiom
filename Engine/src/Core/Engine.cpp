#include "axiom/Core/Engine.h"
#include "axiom/Core/Game.h"
#include "axiom/Platform/Window.h"

namespace axiom
{

    Engine::Engine(EngineConfig config, IWindow& window)
    {
        m_engineConfig = config;
        m_window = &window;
    }

    int Engine::Run(Game *game)
    {
        std::printf("Starting Axiom Engine...\n");
        
        InitializeModules();
        
        while (!m_window->ShouldClose())
        {
            Update();
            Render(); 
        }

        m_window->CloseWindow();

        return 0;    
    }

    void Engine::Update()
    {
        m_window->Update();

        for (auto& [type, module] : m_engineModules)
        {
            module->Update();
        }
        
    }

    void Engine::Render()
    {
        m_window->Render();

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