#pragma once

#include "Platform/ApplicationWindow.h"
#include "Rendering/RenderDevice.h"
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
    protected:
        
        AppConfig m_appConfig;
        
        UniquePtr<IApplicationWindow> m_applicationWindow;

        std::unordered_map<std::type_index, std::unique_ptr<EngineModule>> m_engineModules;

        virtual void OnApplicationStart();

        virtual void PoolEvents();
        virtual void Update();
        virtual void Render();

        virtual void RegisterModules();
        virtual void InitializeModules();

        template<typename T>
        void RegisterModule();

        template<typename T>
        void UnregisterModule();
    };
}