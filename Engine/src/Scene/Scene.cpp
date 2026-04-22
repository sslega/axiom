#include "Scene/Scene.h"
#include "Scene.h"

namespace axiom
{
 
    Scene::Scene(const String& name)
    {
        m_name = name;
    }

    Entity& Scene::CreateEntity()
    {
        UniquePtr<Entity> entity = std::make_unique<Entity>();
        Entity* entityPtr = entity.get();
        m_entities[entity->id] = std::move(entity);
        entityPtr->Register();
        entityPtr->Initialize();
        return *entityPtr;
    }

    Entity& Scene::CreateEntity(String name)
    {
        UniquePtr<Entity> entity = std::make_unique<Entity>(name);
        Entity* entityPtr = entity.get();
        m_entities[entity->id] = std::move(entity);
        entityPtr->Register();
        entityPtr->Initialize();
        return *entityPtr;
    }

    void Scene::DestroyEntity(Entity& entity)
    {
        auto it = m_entities.find(entity.id);
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