#pragma once

#include "Reflection/Reflection.h"
#include "Serialization/Archive.h"

namespace axiom
{
    template<class U> 
    constexpr FieldType FieldTypeTag(U*)
    {
        static_assert(sizeof(U) == 0, "No FieldTypeTag specialization for this type");
        return {};
    }    

    constexpr FieldType FieldTypeTag(float*) { return FieldType::Float; }
    constexpr FieldType FieldTypeTag(int*)   { return FieldType::Int;   }
    constexpr FieldType FieldTypeTag(bool*)  { return FieldType::Bool;  }
    constexpr FieldType FieldTypeTag(Vec3*)  { return FieldType::Vec3;  }
    constexpr FieldType FieldTypeTag(String*){ return FieldType::String;}

    template<class U> 
    constexpr FieldType MapFieldType() 
    { 
        return FieldTypeTag(static_cast<U*>(nullptr)); 
    }

    template<class T, class U>
    FieldDescriptor MakeField(const char* name, U T::* m)
    {
        FieldDescriptor f;
        f.name = name;
        f.type = MapFieldType<U>();
        f.get  = [m](const void* o)                             { return FieldValue(static_cast<const T*>(o)->*m); };
        f.set  = [m](void* o, const FieldValue& v)              { static_cast<T*>(o)->*m = std::get<U>(v); };
        f.serialize = [name, m](Archive& ar, const void* o)     { ar.Write(name, static_cast<const T*>(o)->*m); };
        f.deserialize = [name, m](Archive& ar, void* o)         { ar.Read(name, static_cast<T*>(o)->*m); };
        return f;
    }

    template<class T, class U>
    FieldDescriptor MakeField(const char* name, U (T::*getter)() const, void (T::*setter)(U))
    {
        FieldDescriptor f;
        f.name = name;
        f.type = MapFieldType<U>();
        f.get = [getter](const void* o)                     { return FieldValue((static_cast<const T*>(o)->*getter)()); };
        f.set = [setter](void* o, const FieldValue& v)      { (static_cast<T*>(o)->*setter)(std::get<U>(v)); };
        f.serialize = [name, getter](Archive& ar, const void* o) { ar.Write(name, (static_cast<const T*>(o)->*getter)()); };
        f.deserialize = [name, getter, setter](Archive& ar, void* o)
        {
            T* obj = static_cast<T*>(o);
            U local = (obj->*getter)();   // seed with current value
            ar.Read(name, local);         // overwrites local only if key present
            (obj->*setter)(local);        // push back through the setter
        };

        return f;
    }

    template<class T, class R>
    FieldDescriptor MakeResourceField(const char* name, SharedPtr<R> T::* m)
    {
        FieldDescriptor f;
        f.name = name;
        f.serialize   = [name, m](Archive& ar, const void* o) { ar.Write(name, static_cast<const T*>(o)->*m); };
        f.deserialize = [name, m](Archive& ar, void* o)       { ar.Read (name, static_cast<T*>(o)->*m);       };
        return f;
    }

}