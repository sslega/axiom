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

    void Entity::OnUnregister()
    {
        for (auto& [id, component] : m_components)
            component->Unregister();
    }

    void Entity::OnShutdown()
    {
        for (auto& [id, component] : m_components)
            component->Shutdown();
    }

    void Entity::OnUpdate(float deltaTime)
    {
        for (auto& [id, component] : m_components)
            component->Update(deltaTime);
    }

    void Entity::OnBeginFrame()
    {
        for (auto& [id, component] : m_components)
            component->BeginFrame();
    }

    void Entity::OnRender()
    {
        for (auto& [id, component] : m_components)
            component->Render();
    }

    void Entity::OnEndFrame()
    {
        for (auto& [id, component] : m_components)
            component->EndFrame();
    }
}
