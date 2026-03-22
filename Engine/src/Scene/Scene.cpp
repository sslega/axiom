#include "Scene/Scene.h"

namespace axiom
{
 
    Scene::Scene()
    {
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
   
}