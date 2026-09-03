#include "ReflectedSerialization.h"
#include "Serialization/Archive.h"
#include "Reflection/Reflection.h"

namespace axiom
{
    void SerializeReflected(Archive& ar, const void* obj, const TypeDescriptor& desc)
    {
        for (const FieldDescriptor& f : desc.fields)
        {
            FieldValue v = f.get(obj);
            switch (f.type)
            {
                case FieldType::Float:  { float  x = std::get<float>(v);  ar.Write(f.name, x); break; }
                case FieldType::Int:    { int    x = std::get<int>(v);    ar.Write(f.name, x); break; }
                case FieldType::Bool:   { bool   x = std::get<bool>(v);   ar.Write(f.name, x); break; }
                case FieldType::Vec3:   { Vec3   x = std::get<Vec3>(v);   ar.Write(f.name, x); break; }
                case FieldType::String: { String x = std::get<String>(v); ar.Write(f.name, x); break; }
            }
        }
    }
    
    void DeserializeReflected(Archive &ar, void *obj, const TypeDescriptor &desc)
    {
        for (const FieldDescriptor& f : desc.fields)
        {
            switch (f.type)
            {
                case FieldType::Float:  { float     x = std::get<float>(f.get(obj));    ar.Read(f.name, x); f.set(obj, FieldValue(x)); break; }
                case FieldType::Int:    { int       x = std::get<int>(f.get(obj));      ar.Read(f.name, x); f.set(obj, FieldValue(x)); break; }
                case FieldType::Bool:   { bool      x = std::get<bool>(f.get(obj));     ar.Read(f.name, x); f.set(obj, FieldValue(x)); break; }
                case FieldType::Vec3:   { Vec3      x = std::get<Vec3>(f.get(obj));     ar.Read(f.name, x); f.set(obj, FieldValue(x)); break; }
                case FieldType::String: { String    x = std::get<String>(f.get(obj));   ar.Read(f.name, x); f.set(obj, FieldValue(x)); break; }
            }
        }
    }
}