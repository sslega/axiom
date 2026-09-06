#include "ReflectedSerialization.h"
#include "Serialization/Archive.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    void SerializeReflected(Archive& ar, const void* obj, const TypeDescriptor& desc)
    {
        for (const FieldDescriptor& f : desc.fields)
        {
            f.serialize(ar, obj);
        }
    }
    
    void DeserializeReflected(Archive &ar, void *obj, const TypeDescriptor &desc)
    {
        for (const FieldDescriptor& f : desc.fields)
        {
            f.deserialize(ar, obj);
        }
    }
}