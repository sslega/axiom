#include "Scene/Entity.h"
#include "Entity.h"
#include "Serialization/Archive.h"

namespace axiom
{

    Entity::~Entity()
    {
        for (auto& [id, component] : m_components)
            component->Unregister();
    }

    void Entity::AddComponent(UniquePtr<Component> component)
    {
        auto typeId = std::type_index(typeid(*component));
        Component* ptr = component.get();
        ptr->m_entity = this;
        m_components[typeId] = std::move(component);
        ptr->Register();
        if (m_live)
        {
            ptr->Initialize();
        }
    }

    UniquePtr<Component> Entity::RemoveComponent(std::type_index type)
    {
        auto it = m_components.find(type);
        if (it != m_components.end())
        {
            if (m_live)
            {
                it->second->Shutdown();
            }
            it->second->Unregister();
            UniquePtr<Component> base = std::move(it->second);
            m_components.erase(it);
            return UniquePtr<Component>(base.release());
        }
        return nullptr;
    }

    bool Entity::HasComponent(std::type_index type) const
    {
        auto it = m_components.find(type);
        return it != m_components.end();
    }

    Vector<Component*> Entity::GetComponents() const
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

    Vector<Component*> Entity::FindComponentsRequiring(std::type_index componentType) const
    {
        Vector<Component*> componentsRequiring;
        for (auto& [type, component] : m_components)
        {
            if(component->RequiresComponent(componentType)) componentsRequiring.emplace_back(component.get());
        }
        return componentsRequiring;
    }

    Vector<std::type_index> Entity::FindMissingRequirements(const Component& component) const
    {
        Vector<std::type_index> missingRequirements;
        const Vector<std::type_index>& requiredComponents = component.GetRequiredComponents();
        for(auto& componentType : requiredComponents)
        {
            if(!HasComponent(componentType)) missingRequirements.emplace_back(componentType);
        }
        return missingRequirements;
    }

    void Entity::Deserialize(Archive &ar)
    {
        Object::Deserialize(ar);
        ar.Read("name", m_name);
    }

    void Entity::Serialize(Archive &ar)
    {
        Object::Serialize(ar);
        ar.Write("name", m_name);
    }

    void Entity::OnInitialize()
    {
        for (auto& [id, component] : m_components)
            component->Initialize();
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
