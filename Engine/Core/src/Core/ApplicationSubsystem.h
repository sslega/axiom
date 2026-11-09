#pragma once

#include "Subsystem.h"

namespace axiom
{
    class SubsystemHost;

    /// Base class of an engine-wide service owned by a SubsystemHost, which in practice is Application.
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
    ///
    /// A .cpp that calls GetSubsystem() includes Core/SubsystemHost.h, which defines it.
    class ApplicationSubsystem : public Subsystem
    {
        friend class SubsystemHost;

    public:
        ApplicationSubsystem(SubsystemHost& host);
        virtual ~ApplicationSubsystem() = default;

    protected:
        SubsystemHost& m_host;

        template <typename T>
        T& GetSubsystem();

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
