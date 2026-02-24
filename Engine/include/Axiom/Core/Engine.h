#pragma once

#include "axiom/Platform/ApplicationWindow.h"
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
        Engine(EngineConfig engineConfig, ApplicationWindowDesc windowConfig);
        ~Engine() = default;
        
        int Run(Game* game);

    private:
        EngineConfig m_engineConfig;
        
        UniquePtr<IApplicationWindow> m_applicationWindow;

        std::unordered_map<std::type_index, std::unique_ptr<EngineModule>> m_engineModules;

        void PoolEvents();
        void Update();
        void Render();

        void InitializeModules();

        template<typename T>
        void RegisterModule();

        template<typename T>
        void UnregisterModule();
    };
}