#include "Scene/Component.h"

namespace axiom
{
    Component::Component()
    {
    } 

    Entity* Component::GetEntity() const
    {
        return m_entity;
    }

}

