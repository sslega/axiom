#include "ReflectionSubsystem.h"
#include "Log.h"
#include "Scene/TransformComponent.h"
#include "Renderer/MeshComponent.h"
#include "Renderer/CameraComponent.h"
#include "Renderer/LightComponent.h"

namespace axiom
{
    axiom::ReflectionSubsystem::ReflectionSubsystem(Application &application)
    : ApplicationSubsystem(application)
    {
    }
    
    void ReflectionSubsystem::OnInitialize()
    {
        m_typeRegistry.Register<TransformComponent>();
        m_typeRegistry.Register<MeshComponent>();
        m_typeRegistry.Register<DirectionalLightComponent>();
        m_typeRegistry.Register<CameraComponent>();

        const TypeDescriptor* desc = m_typeRegistry.Get<TransformComponent>();
        Log::Info("TransformComponent fields: {}", desc->fields.size());
    }
}