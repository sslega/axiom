#include "Scene/Scene.h"

namespace axiom
{
 
    Scene::Scene(const String& name)
    {
        m_name = name;
    }

    Entity* Scene::CreateEntity()
    {
        UniquePtr<Entity> entity = std::make_unique<Entity>();
        Entity* entityPtr = entity.get();        
        m_entities[entity->id] = std::move(entity);
        return entityPtr;
    }

    void Scene::DestroyEntity(Entity* entity)
    {
        m_entities.erase(entity->id);
    }

    const String &Scene::GetName() const
    {
        return m_name;
    }
}