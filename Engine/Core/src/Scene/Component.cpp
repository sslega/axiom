#include "Scene/Component.h"
#include "Component.h"
#include "Reflection/Reflection.h"
#include "Serialization/Archive.h"
#include "Serialization/ReflectedSerialization.h"
#include <algorithm>

namespace axiom
{
    Entity& Component::GetEntity() const
    {
        return *m_entity;
    }

    const Vector<std::type_index>& Component::GetRequiredComponents() const
    {
        static const Vector<std::type_index> required = {};
        return required;
    }

    bool Component::RequiresComponent(std::type_index componentType) const
    {
        return std::ranges::find(GetRequiredComponents(), componentType) != GetRequiredComponents().end();
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
