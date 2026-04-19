#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"
#include "Application.h"
#include "ApplicationModule.h"
#include "Platform/ApplicationWindow.h"
#include "Renderer/RenderTypes.h"
#include "Renderer/GraphicsDevice.h"
#include "Event/Event.h"

#include "Core/Timestep.h"

#include <unordered_map>
#include <typeindex>

namespace axiom
{
    class Input;
    class Log;

    struct AppConfig
    {
        GraphicsDevice::API renderAPI = GraphicsDevice::API::OpenGL;
        AppWindowConfig windowConfig;
    };

    class Application : public EventDispatcher
    {
        friend class Input;
        friend class Log;

    public:
        Application(AppConfig appConfig);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        int Run();

        const GraphicsDevice::API GetRenderAPI() const;
        ApplicationWindow& GetApplicationWindow();

        static inline Application& Get() { return *s_instance; }

        template <typename T>
        T* GetModule()
        {
            return const_cast<T*>(std::as_const(*this).GetModule<T>());
        }

        template <typename T>
        const T* GetModule() const
        {
            auto it = m_engineModules.find(TypeID<T>());
            if (it == m_engineModules.end())
                return nullptr;
            return static_cast<const T*>(it->second.get());
        }

    protected:
        AppConfig m_appConfig;
        UniquePtr<ApplicationWindow> m_applicationWindow;
        TypeMap<UniquePtr<ApplicationModule>> m_engineModules;
        Vector<std::type_index> m_moduleOrder; // tracks registration order
        UniquePtr<Input> m_input;
        UniquePtr<Log> m_log;

        // User override hooks — override these in your Application subclass
        virtual void OnApplicationRun()    {}
        virtual void OnUpdate(Timestep delta) {}
        virtual void OnRender()            {}
        virtual void OnRegisterModules()   {}
        virtual void OnInitializeModules() {}

        template <typename T>
        T* RegisterModule()
        {
            Log::Info("Registering module: {}", typeid(T).name());
            UniquePtr<T> module = MakeUnique<T>(*this);
            T* ptr = module.get();
            std::type_index id = TypeID<T>();
            m_engineModules[id] = std::move(module);
            m_moduleOrder.push_back(id);
            return ptr;
        }

        template <typename T>
        void UnregisterModule()
        {
            std::type_index id = TypeID<T>();
            m_engineModules.erase(id);
            m_moduleOrder.erase(
                std::remove(m_moduleOrder.begin(), m_moduleOrder.end(), id),
                m_moduleOrder.end()
            );
        }

    private:
        // Engine loop drivers — sealed, not overridable
        void PoolEvents();
        void Update();
        void Render();
        void DebugRender();
        void RegisterModules();
        void InitializeModules();
        void ShutdownModules();

        static Application* s_instance;
        TimePoint m_lastUpdateTime;
        TimePoint m_lastRenderTime;
        Timestep m_dt; 
        // float m_fps;
    };
}
