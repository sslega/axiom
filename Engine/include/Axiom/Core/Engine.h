#pragma once

#include "Window.h"
#include "Game.h"

namespace axiom
{
    struct EngineConfig
    {
    };

    class Engine
    {
    public:
        Engine(EngineConfig config, IWindow& window);
        ~Engine() = default;
        
        int Run(Game* game);

    private:
        EngineConfig m_config;
        IWindow* m_window;

        void Tick();
        void Render();
    };
}