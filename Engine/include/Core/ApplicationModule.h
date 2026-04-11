#pragma once

namespace axiom
{
    class Application;

    class ApplicationModule
    {
    public:
        ApplicationModule(Application& application);
        virtual ~ApplicationModule() = default;

        // Non-virtual public entry points — called by the engine
        void Initialize();
        void Shutdown();

        virtual void OnRegister();
        virtual void OnUnregister();

        Application& GetApp() const;

    protected:
        virtual void OnInitialize() {}
        virtual void OnShutdown()   {}
        virtual void OnUpdate()     {}
        virtual void OnRender()     {}

        Application& m_application;

    private:
        friend class Application;
        void Update() { OnUpdate(); }
        void Render() { OnRender(); }
    };
}
