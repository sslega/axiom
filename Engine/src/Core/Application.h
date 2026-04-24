#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"
#include "Application.h"
#include "ApplicationModule.h"
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
    class FileSystemModule;
    class ResourceModule;
    class SceneModule;
    class RenderModule;
    class ImGuiModule;


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
        Input& GetInput() const { return *m_input; }
        Log& GetLog() const { return *m_log; }

        static inline Application& Get() { return *s_current; }

        template <typename T>
        T& GetModule()
        {
            auto it = m_engineModules.find(TypeID<T>());
            AX_ASSERT(it != m_engineModules.end(), "Module not registered");
            return *reinterpret_cast<T*>(it->second.get());
        }

        template <typename T>
        const T& GetModule() const
        {
            auto it = m_engineModules.find(TypeID<T>());
            AX_ASSERT(it != m_engineModules.end(), "Module not registered");
            return *reinterpret_cast<const T*>(it->second.get());
        }

        template <typename T>
        bool HasModule() const
        {
            return m_engineModules.find(TypeID<T>()) != m_engineModules.end();
        }

    protected:
        
        AppConfig m_appConfig;
        UniquePtr<ApplicationWindow> m_applicationWindow;
        TypeMap<UniquePtr<ApplicationModule>> m_engineModules;
        Vector<std::type_index> m_moduleOrder; // tracks registration order

        UniquePtr<Input> m_input;
        UniquePtr<Log> m_log;

        FileSystemModule* fileSystemModule = nullptr;
        ResourceModule* resourceModule = nullptr;
        SceneModule* sceneModule = nullptr;
        RenderModule* renderModule = nullptr;
        ImGuiModule* imGuiModule = nullptr;

        // User override hooks — override these in your Application subclass
        virtual void OnApplicationRun() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnBeginFrame() {}
        virtual void OnRender() {}
        virtual void OnEndFrame();
        virtual void OnRegisterModules() {}
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
            ptr->Register();
            return ptr;
        }

        template <typename T>
        void UnregisterModule()
        {
            std::type_index id = TypeID<T>();
            auto it = m_engineModules.find(id);
            if (it != m_engineModules.end())
                it->second->Unregister();
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
        void RegisterModules();
        void InitializeModules();
        void ShutdownModules();

        inline static Application& GetCurrent() {
            AX_ASSERT(s_current, "No Application in scope");
            return *s_current;
        }

        static Application* s_current;
        TimePoint m_lastUpdateTime;
        Timestep m_dt; 
    };
}
