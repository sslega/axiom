#include "axiom/Core/EngineModule.h"
#include "Axiom/Core/Engine.h"
#include <cstdio>
#include <typeinfo>

namespace axiom
{
    EngineModule::EngineModule(Engine& engine)
        : m_engine(engine)
    {
    }

    bool EngineModule::Initialize()
    {
        return true;
    }

    void EngineModule::OnRegister()
    {
        printf("Module registered: %s\n", typeid(*this).name());
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