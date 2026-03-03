#pragma once

#include "../Platform/ApplicationWindow.h"
#include "../Renderer/Renderer.h"
#include "Application.h"
#include "EngineModule.h"
#include <unordered_map>
#include <typeindex>

namespace axiom
{
    struct EngineConfig
    {
        RenderAPI renderAPI = RenderAPI::OpenGL;
    };

    class Engine
    {
    public:
        Engine(EngineConfig engineConfig, ApplicationWindowConfig windowConfig);
        ~Engine() = default;
        
        int Run(Application* game);

        const RenderAPI GetRenderAPI() const ;
        const IApplicationWindow& GetApplicationWindow() const;

    private:
        EngineConfig m_engineConfig;
        
        UniquePtr<IApplicationWindow> m_applicationWindow;

        std::unordered_map<std::type_index, std::unique_ptr<EngineModule>> m_engineModules;

        void PoolEvents();
        void Update();
        void Render();

        void RegisterModules();
        void InitializeModules();

        template<typename T>
        void RegisterModule();

        template<typename T>
        void UnregisterModule();
    };
}