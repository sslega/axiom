#pragma once

// #include "Axiom/Core/Engine.h"

namespace axiom
{
    class Engine;
    
    class EngineModule
    {
    public:
        EngineModule(Engine& engine);
        virtual ~EngineModule() = default;

        virtual bool Initialize();
        virtual void OnRegister();
        virtual void OnUnregister();
        virtual void Shutdown();
        virtual void Update() ;
        virtual void Render();
        
    protected:
        Engine& m_engine;
    };
}