#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"
#include "Application.h"
#include "ApplicationSubsystem.h"
#include "Platform/ApplicationWindow.h"
#include "Renderer/RenderTypes.h"
#include "Renderer/GraphicsDevice.h"
#include "Event/Event.h"
#include "Log.h"

#include "Core/Timestep.h"

#include <unordered_map>
#include <typeindex>

namespace axiom
{
    class Input;
    class Log;
    class FileSubsystem;
    class ResourceSubsystem;
    class WorldSubsystem;
    class RenderSubsystem;
    class ImGuiSubsystem;


    struct AppConfig
    {
        GraphicsDevice::API renderAPI = GraphicsDevice::API::OpenGL;
        AppWindowConfig windowConfig;
    };

    class Application : public EventDispatcher
    {
    public:
        Application(AppConfig appConfig);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        int Run();

        const GraphicsDevice::API GetRenderAPI() const;
        ApplicationWindow& GetApplicationWindow();

        template <typename T>
        T& GetSubsystem()
        {
            auto it = m_applicationSubsystems.find(TypeID<T>());
            AX_ASSERT(it != m_applicationSubsystems.end(), "Module not registered");
            return *reinterpret_cast<T*>(it->second.get());
        }

        template <typename T>
        const T& GetSubsystem() const
        {
            auto it = m_applicationSubsystems.find(TypeID<T>());
            AX_ASSERT(it != m_applicationSubsystems.end(), "Module not registered");
            return *reinterpret_cast<const T*>(it->second.get());
        }

        template <typename T>
        bool HasSubsystem() const
        {
            return m_applicationSubsystems.find(TypeID<T>()) != m_applicationSubsystems.end();
        }

    protected:
        
        AppConfig m_appConfig;
        UniquePtr<ApplicationWindow> m_applicationWindow;
        TypeMap<UniquePtr<ApplicationSubsystem>> m_applicationSubsystems;
        Vector<std::type_index> m_subsystemOrder; // tracks registration order

        UniquePtr<Input> m_input;
        UniquePtr<Log> m_log;

        FileSubsystem* fileSubsystem = nullptr;
        ResourceSubsystem* resourceSubsystem = nullptr;
        WorldSubsystem* worldSubsystem = nullptr;
        RenderSubsystem* renderSubsystem = nullptr;
        ImGuiSubsystem* imGuiSubsystem = nullptr;

        // User override hooks — override these in your Application subclass
        virtual void OnApplicationRun() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnBeginFrame() {}
        virtual void OnRender() {}
        virtual void OnEndFrame();
        virtual void OnRegisterModules() {}
        virtual void OnInitializeModules() {}

        template <typename T>
        T* RegisterSubsystem()
        {
            Log::Info("Registering subsystem: {}", typeid(T).name());
            UniquePtr<T> module = MakeUnique<T>(*this);
            T* ptr = module.get();
            std::type_index id = TypeID<T>();
            m_applicationSubsystems[id] = std::move(module);
            m_subsystemOrder.push_back(id);
            ptr->Register();
            return ptr;
        }

        template <typename T>
        void UnregisterSubsystem()
        {
            std::type_index id = TypeID<T>();
            auto it = m_applicationSubsystems.find(id);
            if (it != m_applicationSubsystems.end())
                it->second->Unregister();
            m_applicationSubsystems.erase(id);
            m_subsystemOrder.erase(
                std::remove(m_subsystemOrder.begin(), m_subsystemOrder.end(), id),
                m_subsystemOrder.end()
            );
        }

    private:
        // Engine loop drivers — sealed, not overridable
        void PoolEvents();
        void Update();
        void Render();
        void RegisterSubsystems();
        void InitializeSubsystems();
        void ShutdownSubsystems();
        virtual void RegisterComponentFactories();

        // inline static Application& GetCurrent() {
        //     AX_ASSERT(s_current, "No Application in scope");
        //     return *s_current;
        // }

        static Application* s_current;
        TimePoint m_lastUpdateTime;
        Timestep m_dt; 
    };
}
