#pragma once
#include "Core/Types.h"
#include "Scene/Entity.h"

namespace axiom
{
    class Scene
    {
        friend class WorldSubsystem;

    public:
        Scene(const String& name);
        ~Scene() = default;

        SharedPtr<Entity> CreateEntity();
        void AddEntity(SharedPtr<Entity> entity);
        SharedPtr<Entity> RemoveEntity(Entity& entity);

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

        Vector<Entity*> GetEntities() const;
        void GetEntities(Vector<Entity*>& outEntities) const;
        String GetUniqueName(const String& baseName);

    protected:
        virtual void OnRegister()   {}
        virtual void OnUnregister() {}
        virtual void FileSubsystem() {}
        virtual void OnShutdown()   {}
        virtual void OnUpdate(float deltaTime);
        virtual void OnBeginFrame();
        virtual void OnRender();
        virtual void OnEndFrame();

    private:
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { FileSubsystem(); }
        void Shutdown()              { OnShutdown(); }
        void Update(float deltaTime) { OnUpdate(deltaTime); }
        void BeginFrame()            { OnBeginFrame(); }
        void Render()                { OnRender(); }
        void EndFrame()              { OnEndFrame(); }

        String m_name;
        StringMap<SharedPtr<Entity>> m_entities;
        StringMap<int> m_nameCounters;
    };
}
