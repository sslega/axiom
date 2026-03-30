#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"
#include "Application.h"
#include "ApplicationModule.h"
#include "Platform/ApplicationWindow.h"
#include "Rendering/RenderTypes.h"
// #include "Renderer/RendererAPI.h"
// #include "Rendering/RenderDevice.h"

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
        const ApplicationWindow& GetApplicationWindow() const;

        template <typename T>
        T* GetModule()
        {
            auto it = m_engineModules.find(TypeID<T>());
            if (it == m_engineModules.end())
            {
                return nullptr;
            }
            T* result = static_cast<T*>(it->second.get());
            AX_ASSERT(result, "Module not registered.");
            return result;
        }
    protected:
        
        AppConfig m_appConfig;
        
        UniquePtr<ApplicationWindow> m_applicationWindow;

        TypeMap<UniquePtr<ApplicationModule>> m_engineModules;

        virtual void OnApplicationRun();

        virtual void PoolEvents();
        virtual void Update();
        virtual void Render();

        virtual void RegisterModules();
        virtual void OnRegisterModules();
        virtual void InitializeModules();
        virtual void OnInitializeModules();

        template <typename T>
        T* RegisterModule()
        {
            printf("Registering module: %s\n", typeid(T).name());
            UniquePtr<T> module = MakeUnique<T>(*this);
            T* ptr = module.get(); 
            m_engineModules[TypeID<T>()] = std::move(module);
            return ptr;
        }

        template <typename T>
        void UnregisterModule()
        {
            m_engineModules.erase(typeid(T));
        }
    };
}