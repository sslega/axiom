#pragma once

#include "Assert.h"
#include "Window.h"

namespace ax
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
        
        int Run();

    private:
        EngineConfig m_config;
        Window m_window;

        void Tick();
        void Render();
    };
}