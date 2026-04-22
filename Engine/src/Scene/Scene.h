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

        Entity& CreateEntity();
        Entity& CreateEntity(String name);
        void DestroyEntity(Entity& entity);

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
    protected:
        virtual void OnRegister()   {}
        virtual void OnUnregister() {}
        virtual void OnInitialize() {}
        virtual void OnShutdown()   {}
        virtual void OnUpdate(float deltaTime);
        virtual void OnBeginFrame();
        virtual void OnRender();
        virtual void OnEndFrame();

    private:
        friend class SceneModule;
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { OnInitialize(); }
        void Shutdown()              { OnShutdown(); }
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame()            { OnBeginFrame(); }
        void Render()                { OnRender(); }
        void EndFrame()              { OnEndFrame(); }

        String m_name;
        UnorderedMap<Entity::ID, UniquePtr<Entity>> m_entities;
    };
}
