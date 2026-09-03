#include "Scene/Component.h"
#include "Component.h"
#include "Reflection/Reflection.h"
#include "Serialization/Archive.h"
#include "Serialization/ReflectedSerialization.h"

namespace axiom
{
    Entity& Component::GetEntity() const
    {
        return *m_entity;
    }

    void Component::Deserialize(Archive &ar)
    {
        if (const TypeDescriptor* d = Reflection::Get().Find(typeid(*this)))
        {
            DeserializeReflected(ar, this, *d);
        }
    }

    void Component::Serialize(Archive &ar)
    {
        if (const TypeDescriptor* d = Reflection::Get().Find(typeid(*this)))
        {
            SerializeReflected(ar, this, *d);
        }
    }
}
