#pragma once

#include "Core.h"
#include "Application.h"
#include "EngineModule.h"
#include "Platform/ApplicationWindow.h"
#include "Rendering/RenderDevice.h"

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

        TypeMap<UniquePtr<EngineModule>> m_engineModules;

        virtual void OnApplicationStart();

        virtual void PoolEvents();
        virtual void Update();
        virtual void Render();

        virtual void RegisterModules();
        virtual void InitializeModules();

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
    };
}