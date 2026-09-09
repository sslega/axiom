#include "Scene/Entity.h"
#include "Entity.h"

namespace axiom
{

    void Entity::AddComponent(UniquePtr<Component> component)
    {
        auto typeId = std::type_index(typeid(*component));
        Component* ptr = component.get();
        ptr->m_entity = this;
        m_components[typeId] = std::move(component);
        ptr->Register();
        ptr->Initialize();
    }

    Vector<Component *> Entity::GetComponents() const
    {
        Vector<Component*> results;
        GetComponents(results);
        return results;
    }

    void Entity::GetComponents(Vector<Component *> &outComponents) const
    {
        for (auto& [type, component] : m_components)
        {
            outComponents.push_back(component.get());
        }         
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
