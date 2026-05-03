#include "Scene/Component.h"
#include "Component.h"

namespace axiom
{
    Entity& Component::GetEntity() const
    {
        return *m_entity;
    }
}
