#include "axiom/core/Engine.h"
#include "axiom/core/Window.h"
#include "axiom/core/Game.h"

namespace axiom
{
    Engine::Engine(EngineConfig config)
    : m_config(config)
    {
    }

    int Engine::Run(Game* game)
    {
        // bool shouldClose = false;
        // while (!shouldClose)
        // {
        //     Tick();
        //     Render(); 
        //     shouldClose = m_window.ShouldClose();
        // }

        // m_window.CloseWindow();

        return 0;    
    }

    void Engine::Tick()
    {
        // m_window.Update();
    }

    void Engine::Render()
    {
        // m_window.Render();
    }
}