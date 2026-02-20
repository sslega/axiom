#include "axiom/core/Engine.h"
#include "axiom/core/Window.h"
#include "axiom/core/Game.h"

namespace axiom
{

    Engine::Engine(EngineConfig config, IWindow& window)
    {
        m_config = config;
        m_window = &window;
    }

    int Engine::Run(Game *game)
    {
        while (!m_window->ShouldClose())
        {
            Tick();
            Render(); 
        }

        m_window->CloseWindow();

        return 0;    
    }

    void Engine::Tick()
    {
        m_window->Update();
    }

    void Engine::Render()
    {
        m_window->Render();
    }
}