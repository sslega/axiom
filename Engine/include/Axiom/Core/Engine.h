#pragma once

#include "axiom/Platform/Window.h"
#include "axiom/Renderer/Renderer.h"
#include "axiom/Core/Game.h"
#include <unordered_map>
#include <typeindex>

namespace axiom
{
    struct EngineConfig
    {
    };

    class Engine
    {
    public:
        Engine(EngineConfig engineConfig, IWindow& window);
        ~Engine() = default;
        
        int Run(Game* game);

    private:
        EngineConfig m_engineConfig;
        
        IWindow* m_window;

        std::unordered_map<std::type_index, std::unique_ptr<EngineModule>> m_engineModules;

        void Update();
        void Render();

        void InitializeModules();

        template<typename T>
        void RegisterModule();

        template<typename T>
        void UnregisterModule();
    };
}