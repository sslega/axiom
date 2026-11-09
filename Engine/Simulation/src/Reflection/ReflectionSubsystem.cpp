#include "Reflection/ReflectionSubsystem.h"

namespace axiom
{
    axiom::ReflectionSubsystem::ReflectionSubsystem(SubsystemHost& host)
    : ApplicationSubsystem(host)
    {
    }
    
    void ReflectionSubsystem::OnInitialize()
    {
        for (Registrar registrar : m_registrars)
        {
            registrar(m_typeRegistry);
        }
        Reflection::SetInstance(&m_typeRegistry);
    }
    
    void ReflectionSubsystem::OnShutdown()
    {
        Reflection::SetInstance(nullptr);
    }
}