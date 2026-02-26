#include "axiom/Core/EngineModule.h"
#include "Axiom/Core/Engine.h"
#include <cstdio>
#include <typeinfo>

namespace axiom
{
    EngineModule::EngineModule()
    {
    }

    bool EngineModule::Initialize(Engine& engine)
    {
        m_engine = &engine;
        return true;
    }

    void EngineModule::OnRegister()
    {
        std::printf("Module registered: %s\n", typeid(*this).name());
    }

    void EngineModule::OnUnregister()
    {
    }

    void EngineModule::Shutdown()
    {
    }

    void EngineModule::Update()
    {
    }

    void EngineModule::Render()
    {
    }
}