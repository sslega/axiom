#include "Axiom/Core/Engine.h"
#include "Axiom/Core/Window.h"

using namespace ax;

Engine::Engine(EngineConfig config)
: m_config(config)
, m_window(m_config.windowConfig)
{
}

int Engine::Run()
{
    bool shouldClose = false;
    while (!shouldClose)
    {
        Tick();
        Render(); 
        shouldClose = m_window.ShouldClose();
    }

    m_window.CloseWindow();

    return 0;    
}

void Engine::Tick()
{
    m_window.Update();
}
void Engine::Render()
{
    m_window.Render();
}
