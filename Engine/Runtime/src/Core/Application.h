#pragma once

#include "Core/Types.h"
#include "Core/Assert.h"
#include "Core/Timestep.h"
#include "Core/Log.h"
#include "Core/SubsystemHost.h"

#include "Platform/ApplicationWindow.h"

#include "Renderer/RenderTypes.h"
#include "Renderer/GraphicsDevice.h"

#include "Event/Event.h"

namespace axiom
{
    class IInput;
    class Log;
    class FileSubsystem;
    class ResourceSubsystem;
    class WorldSubsystem;
    class RenderSubsystem;
    class ImGuiSubsystem;
    class ReflectionSubsystem;


    struct AppConfig
    {
        GraphicsDevice::API renderAPI = GraphicsDevice::API::OpenGL;
        AppWindowConfig windowConfig;
    };

    /// Base class of each executable. Owns the window, input, log and all subsystems, and runs the main loop.
    ///
    /// Subclass it and override the `On*` hooks. At startup they run in this order:
    /// - OnRegisterModules(): subsystems exist but aren't initialized. Mount paths here.
    /// - OnInitializeModules(): subsystems are ready. Build the UI here.
    /// - OnApplicationRun(): the last step before the loop. Load the first scene here.
    ///
    /// @code{.cpp}
    /// void Editor::OnInitializeModules()
    /// {
    ///     m_editorUI = MakeUnique<EditorUI>(GetSubsystem<RenderSubsystem>(), GetSubsystem<WorldSubsystem>(), GetSubsystem<FileSubsystem>());
    /// }
    /// @endcode
    class Application : public EventDispatcher, public SubsystemHost
    {
    public:
        Application(AppConfig appConfig);
        ~Application();

        Application(const Application&) = delete;
        Application& operator=(const Application&) = delete;

        int Run();

        const GraphicsDevice::API GetRenderAPI() const;
        ApplicationWindow& GetApplicationWindow();

    protected:
        // The parts of Run(), for a host that steps the frames itself.
        void Startup();
        void RunFrame();
        void Shutdown();

        AppConfig m_appConfig;
        UniquePtr<ApplicationWindow> m_applicationWindow;

        UniquePtr<IInput> m_input;
        UniquePtr<Log> m_log;

        FileSubsystem* fileSubsystem = nullptr;
        ResourceSubsystem* resourceSubsystem = nullptr;
        WorldSubsystem* worldSubsystem = nullptr;
        RenderSubsystem* renderSubsystem = nullptr;
        ImGuiSubsystem* imGuiSubsystem = nullptr;
        ReflectionSubsystem* reflectionSubsystem = nullptr;

        // User override hooks — override these in your Application subclass
        virtual void OnApplicationRun() {}
        virtual void OnUpdate(float deltaTime) {}
        virtual void OnBeginFrame() {}
        virtual void OnRender() {}
        virtual void OnEndFrame();
        virtual void OnRegisterModules() {}
        virtual void OnInitializeModules() {}

    private:
        // Only the main loop drives these, so subclasses can't.
        using SubsystemHost::InitializeSubsystems;
        using SubsystemHost::ShutdownSubsystems;
        using SubsystemHost::UpdateSubsystems;
        using SubsystemHost::BeginFrameSubsystems;
        using SubsystemHost::RenderSubsystems;
        using SubsystemHost::EndFrameSubsystems;
        using SubsystemHost::DestroySubsystems;

        // Engine loop drivers — sealed, not overridable
        void PoolEvents();
        void Update();
        void Render();
        void RegisterSubsystems();

        static Application* s_current;
        TimePoint m_lastUpdateTime;
        Timestep m_dt; 
    };
}
