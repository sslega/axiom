#include "Scene/Scene.h"
#include "Scene.h"

namespace axiom
{
 
    Scene::Scene(const String& name)
    {
        m_name = name;
    }

    SharedPtr<Entity> Scene::CreateEntity()
    {
        SharedPtr<Entity> entity = std::make_unique<Entity>();
        m_entities[entity->GetID()] = entity;
        entity->Register();
        entity->Initialize();
        return entity;
    }

    void Scene::DestroyEntity(Entity& entity)
    {
        auto it = m_entities.find(entity.GetID());
        if (it != m_entities.end())
        {
            it->second->Shutdown();
            it->second->Unregister();
            m_entities.erase(it);
        }
    }

    const String& Scene::GetName() const
    {
        return m_name;
    }

    Vector<Entity *> Scene::GetEntities() const
    {
        Vector<Entity*> results;
        GetEntities(results);
        return results;
    }

    void Scene::GetEntities(Vector<Entity *> &outEntities) const
    {
        for(auto& [id, entity] : m_entities)
        {
            outEntities.push_back(entity.get());
        }
    }

    void Scene::OnUpdate(float deltaTime)
    {
        for (auto& [id, entity] : m_entities)
            entity->Update(deltaTime);
    }

    void Scene::OnBeginFrame()
    {
        for (auto& [id, entity] : m_entities)
            entity->BeginFrame();
    }

    void Scene::OnRender()
    {
        for (auto& [id, entity] : m_entities)
            entity->Render();
    }

    void Scene::OnEndFrame()
    {
        for (auto& [id, entity] : m_entities)
            entity->EndFrame();
    }
}