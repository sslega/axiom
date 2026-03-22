#pragma once
#include "Core/Types.h"
#include "Scene/Entity.h"

namespace axiom
{

    class Scene
    {
    public:
        Scene();
        ~Scene() = default;

        Entity* CreateEntity();
        void DestroyEntity(Entity* entity);
    private:
        UnorderedMap<Entity::ID, UniquePtr<Entity>> m_entities;
    };
}
