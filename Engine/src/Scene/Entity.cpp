#include "Scene/Entity.h"
#include "Entity.h"

namespace axiom
{
    Entity::Entity(String name)
    : id(s_nextId++)
    , m_name(name)
    {
    }

    axiom::Entity::Entity()
    : id(s_nextId++)
    {
        m_name = "Entity_" + std::to_string(s_nextId);
    }

    void Entity::OnUpdate(float deltaTime)
    {
        for(auto& component : m_components)
        {
            component.second->OnUpdate(deltaTime);
        }
    }

    void Entity::OnBeginFrame()
    {
        for(auto& component : m_components)
        {
            component.second->OnBeginFrame();
        }
    }

    void Entity::OnRender()
    {
        for(auto& component : m_components)
        {
            component.second->OnRender();
        }
    }

    void Entity::OnEndFrame()
    {
        for(auto& component : m_components)
        {
            component.second->OnEndFrame();
        }
    }
}
