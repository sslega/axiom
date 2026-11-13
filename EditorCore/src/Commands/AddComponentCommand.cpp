#include "AddComponentCommand.h"
#include "Scene/Scene.h"
#include "Scene/Entity.h"
#include "Scene/Component.h"
#include "Reflection/Reflection.h"
#include <algorithm>

namespace axiom
{
    AddComponentCommand::AddComponentCommand(Entity& entity, const TypeDescriptor& typeDescriptor)
    : m_entity(entity)
    {
        Collect(typeDescriptor);

        // Collect marks types parent-first to skip repeats; Undo needs them in add order instead,
        // so walking it backwards takes every component off before the ones it requires.
        m_componentTypes.clear();
        for (const UniquePtr<Component>& component : m_components)
        {
            m_componentTypes.push_back(typeid(*component));
        }
    }
    
    void AddComponentCommand::Execute()
    {
        for(auto& component : m_components)
        {
            m_entity.AddComponent(std::move(component));
        }
        m_components.clear();
    }
    
    void AddComponentCommand::Undo()
    {
        for (size_t i = m_componentTypes.size(); i-- > 0; )
        {
            m_components.push_back(m_entity.RemoveComponent(m_componentTypes[i]));
        }
        // Back to add order, so Redo adds requirements before the components that need them.
        std::ranges::reverse(m_components);
    }

    void AddComponentCommand::Collect(const TypeDescriptor& componentType)
    {
        UniquePtr<Component> component = componentType.Create();
        const Vector<std::type_index>& requiredComponents = component->GetRequiredComponents();
        m_componentTypes.push_back(componentType.type);

        for(const std::type_index& requiredComponent : requiredComponents)
        {
            const bool alreadyAdded = std::ranges::find(m_componentTypes, requiredComponent) != m_componentTypes.end();
            if(alreadyAdded || m_entity.HasComponent(requiredComponent))
            {
                continue;
            }

            const TypeDescriptor* descriptor = Reflection::Get().Find(requiredComponent);
            AX_ASSERT(descriptor, "Required component type is not reflected");
            Collect(*descriptor);
        }

        m_components.push_back(std::move(component));
    }
}