#pragma once

#include "Window.h"
#include "Game.h"

namespace axiom
{
    struct EngineConfig
    {
        WindowDesc windowConfig;
    };

    class Engine
    {
    public:
        Engine(EngineConfig config);
        ~Engine() = default;
        
        int Run(Game* game);

    private:
        EngineConfig m_config;
        // Window m_window;

        void Tick();
        void Render();
    };
}