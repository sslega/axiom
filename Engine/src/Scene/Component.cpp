#include "Scene/Component.h"
#include "Component.h"
#include "Serialization/Archive.h"

namespace axiom
{
    Entity& Component::GetEntity() const
    {
        return *m_entity;
    }
}
