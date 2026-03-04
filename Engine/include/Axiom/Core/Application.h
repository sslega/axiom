#pragma once

#include "../Platform/ApplicationWindow.h"
#include "../Renderer/Renderer.h"
#include "Application.h"
#include "EngineModule.h"
#include <unordered_map>
#include <typeindex>

namespace axiom
{
    struct AppConfig
    {
        RenderAPI renderAPI = RenderAPI::OpenGL;
        AppWindowConfig windowConfig;
    };

    class Application
    {
    public:
        Application(AppConfig appConfig);
        ~Application() = default;
        
        int Run();

        const RenderAPI GetRenderAPI() const ;
        const IApplicationWindow& GetApplicationWindow() const;

    private:
        AppConfig m_appConfig;
        
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