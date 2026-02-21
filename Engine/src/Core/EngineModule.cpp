#include "axiom/Core/EngineModule.h"
#include <cstdio>
#include <typeinfo>

namespace axiom
{
    bool EngineModule::Initialize()
    {
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