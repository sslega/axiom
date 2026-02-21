#pragma once

namespace axiom
{
    class EngineModule
    {
    public:
        virtual ~EngineModule() = default;

        virtual bool Initialize();
        virtual void OnRegister();
        virtual void OnUnregister();
        virtual void Shutdown();
        virtual void Update() ;
        virtual void Render();
    };
}