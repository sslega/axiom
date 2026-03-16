#include "Axiom/Core/EngineModule.h"
#include <cstdio>
#include <typeinfo>

namespace axiom
{
    EngineModule::EngineModule(Application& application)
        : m_application(application)
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