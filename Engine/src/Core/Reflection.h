#pragma once
#include "Types.h"
#include "Assert.h"

namespace axiom
{
    class Vec2;
    class Vec3;
    class Vec4;

    enum class FieldType
    {
        Vec2,
        Vec3,
        Vec4,
        Float,
        Bool
    };

    struct FieldDescriptor
    {
        const char* name;
        FieldType type;
        size_t offset;
    };

    struct TypeDescriptor
    {
        const char* name;
        Vector<FieldDescriptor> fields;
    };

    class TypeRegistry 
    {
        public:
            template<typename T> void Register()
            {
                m_descriptors[typeid(T)] = ReflectType<T>();
            }


            template<typename T> const TypeDescriptor* Get() const
            {
                auto it = m_descriptors.find(typeid(T));
                if (it != m_descriptors.end())
                    return &it->second;
                else
                    return nullptr;
            }
        private:
            TypeMap<TypeDescriptor> m_descriptors;
    };

    template<typename T>
    TypeDescriptor ReflectType()
    {
        static_assert(sizeof(T) == 0, "No AX_REFLECT block found for this type");
    }


    template<typename T>
    FieldType DeduceFieldType()
    {
        static_assert(sizeof(T) == 0, "No FieldType specialization for this type");
    }
    
    template<> inline FieldType DeduceFieldType<Vec2>()  { return FieldType::Vec2; }
    template<> inline FieldType DeduceFieldType<Vec3>()  { return FieldType::Vec3; }
    template<> inline FieldType DeduceFieldType<Vec4>()  { return FieldType::Vec4; }    
    template<> inline FieldType DeduceFieldType<float>() { return FieldType::Float; }
    template<> inline FieldType DeduceFieldType<bool>()  { return FieldType::Bool; }

}

#define AX_REFLECT(T)                                       \
template<> inline axiom::TypeDescriptor axiom::ReflectType<T>() {  \
    using SelfType = T;                                     \
    axiom::TypeDescriptor desc;                             \
    desc.name = #T;

#define AX_FIELD(field)                                     \
desc.fields.push_back({#field,                              \
    axiom::DeduceFieldType<decltype(SelfType::field)>(),    \
    offsetof(SelfType, field)});

#define AX_REFLECT_END()  \
return desc;          \
}
