#include "Reflection/ReflectionSubsystem.h"
#include "Reflection/EngineReflection.h"

namespace axiom
{
    axiom::ReflectionSubsystem::ReflectionSubsystem(Application &application)
    : ApplicationSubsystem(application)
    {
    }
    
    void ReflectionSubsystem::OnInitialize()
    {
        RegisterEngineReflection(m_typeRegistry);
        Reflection::SetInstance(&m_typeRegistry);
    }
    
    void ReflectionSubsystem::OnShutdown()
    {
        Reflection::SetInstance(nullptr);
    }
}