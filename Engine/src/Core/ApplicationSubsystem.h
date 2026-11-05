#pragma once

#include "Subsystem.h"

namespace axiom
{
    class Application;

    /// Base class of an engine-wide service owned by Application.
    ///
    /// Derive from it, override the hooks you need, and register it in Application::RegisterSubsystems().
    /// Registration order is also the order the frame hooks run in; shutdown runs in reverse.
    /// Every subsystem exists by the time OnInitialize() runs, but only the earlier ones are initialized.
    ///
    /// @code{.cpp}
    /// void ResourceSubsystem::OnInitialize()
    /// {
    ///     fileSystemModule = &GetSubsystem<FileSubsystem>();
    /// }
    /// @endcode
    class ApplicationSubsystem : public Subsystem
    {
        friend class Application;

    public:
        ApplicationSubsystem(Application& application);
        virtual ~ApplicationSubsystem() = default;

        inline Application& GetApp() const { return m_application; };

    protected:
        Application& m_application;

        template <typename T>
        T& GetSubsystem()
        {
            return GetApp().template GetSubsystem<T>();
        }

    private:
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { OnInitialize(); }
        void Shutdown()              { OnShutdown(); }
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame()            { OnBeginFrame(); }
        void Render()                { OnRender(); }
        void EndFrame()              { OnEndFrame(); }
    };
}
