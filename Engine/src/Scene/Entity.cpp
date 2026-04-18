#include "Scene/Entity.h"

namespace axiom
{
    Entity::Entity(String name)
    : id(s_nextId++)
    , m_name(name)
    {
    }

    axiom::Entity::Entity()
    : id(s_nextId++)
    {
        m_name = "Entity_" + std::to_string(s_nextId);
    }
}

