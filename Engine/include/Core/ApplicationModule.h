#pragma once

namespace axiom
{
    class Application;
    
    class ApplicationModule
    {
    public:
        ApplicationModule(Application& application);
        virtual ~ApplicationModule() = default;

        virtual bool Initialize();
        virtual void OnRegister();
        virtual void OnUnregister();
        virtual void Shutdown();
        virtual void Update() ;
        virtual void Render();
        Application& GetApp() const;
        
    protected:
        Application& m_application;
    };
}