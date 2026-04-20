#include "Scene/Scene.h"
#include "Scene.h"

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

    Entity* Scene::CreateEntity(String name)
    {
        UniquePtr<Entity> entity = std::make_unique<Entity>(name);
        Entity* entityPtr = entity.get();        
        m_entities[entity->id] = std::move(entity);
        return entityPtr;
    }

    void Scene::DestroyEntity(Entity *entity)
    {
        m_entities.erase(entity->id);
    }

    const String &Scene::GetName() const
    {
        return m_name;
    }

    void Scene::OnUpdate(float deltaTime)
    {
        for(auto& entity : m_entities)
        {
            entity.second->OnUpdate(deltaTime);
        }
    }
    
    void Scene::OnBeginFrame()
    {
        for(auto& entity : m_entities)
        {
            entity.second->OnBeginFrame();
        }
    }

    void Scene::OnRender()
    {
        for(auto& entity : m_entities)
        {
            entity.second->OnRender();
        }
    }

    void Scene::OnEndFrame()
    {
        for(auto& entity : m_entities)
        {
            entity.second->OnEndFrame();
        }
    }
}