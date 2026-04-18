#pragma once
#include "Core/Types.h"
#include "Scene/Entity.h"

namespace axiom
{
    class Scene
    {
    public:
        Scene(const String& name);
        ~Scene() = default;

        Entity* CreateEntity();
        Entity* CreateEntity(String name);
        void DestroyEntity(Entity* entity);

        const String& GetName() const;

        template<typename T>
        Vector<T*> GetComponents()
        {
            Vector<T*> results;
            GetComponents(results);
            return results;
        }

        template<typename T>
        void GetComponents(Vector<T*>& outComponents)
        {
            for(auto& [id, entity] : m_entities)
            {
                T* component = entity->GetComponent<T>();
                if(component)
                {
                    outComponents.push_back(component);
                }
            }
        }

    private:
        String m_name;
        UnorderedMap<Entity::ID, UniquePtr<Entity>> m_entities;
    };
}
