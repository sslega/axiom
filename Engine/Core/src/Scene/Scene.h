#pragma once
#include "Core/Types.h"
#include "Scene/Entity.h"

namespace axiom
{
    /// Owns entities by UUID. WorldSubsystem owns the active scene; get it with GetActiveScene().
    ///
    /// RemoveEntity() returns ownership instead of destroying, so an undo command can hold the entity.
    /// Entity order is arbitrary.
    ///
    /// @code{.cpp}
    /// SharedPtr<Entity> entity = scene.CreateEntity();
    /// entity->CreateComponent<TransformComponent>();
    /// Vector<MeshComponent*> meshes = scene.GetComponents<MeshComponent>(); // visits every entity
    /// @endcode
    class Scene
    {
        friend class WorldSubsystem;

    public:
        Scene(const String& name);
        ~Scene() = default;

        SharedPtr<Entity> CreateEntity();
        void AddEntity(SharedPtr<Entity> entity);
        SharedPtr<Entity> RemoveEntity(Entity& entity);

        /// The entity with this ID, or null if it isn't in the scene.
        Entity* FindEntity(const String& id) const;

        /// Removes every entity. Anything still holding an Entity* must drop it.
        void Clear();

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
        virtual void OnInitialize() {}
        virtual void OnShutdown()   {}
        virtual void OnUpdate(float deltaTime);
        virtual void OnBeginFrame();
        virtual void OnRender();
        virtual void OnEndFrame();

    private:
        void Register()              { OnRegister(); }
        void Unregister()            { OnUnregister(); }
        void Initialize()            { OnInitialize(); }
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
