#pragma once

#include "Subsystem.h"

namespace axiom
{
    class Application;

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
