#pragma once

// #include "Axiom/Core/Engine.h"

namespace axiom
{
    class Engine;
    
    class EngineModule
    {
    public:
        EngineModule();
        virtual ~EngineModule() = default;

        virtual bool Initialize(Engine& engine);
        virtual void OnRegister();
        virtual void OnUnregister();
        virtual void Shutdown();
        virtual void Update() ;
        virtual void Render();
        
    protected:
        Engine* m_engine;
    };
}