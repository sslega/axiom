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

        inline Application& GetApp() const { return m_application; };

    protected:
        virtual void OnInitialize()  {}
        virtual void OnShutdown()    {}
        virtual void OnUpdate(float deltaTime){}
        virtual void OnBeginFrame()  {}
        virtual void OnRender()      {}
        virtual void OnEndFrame()    {}

        Application& m_application;

        template <typename T>
        T* GetModule()
        {
            return GetApp().template GetModule<T>();
        }

    private:
        friend class Application;
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame() { OnBeginFrame(); }
        void Render()     { OnRender(); }
        void EndFrame()   { OnEndFrame(); }
    };
}
