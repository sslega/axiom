#pragma once
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Assert.h"
#include "Scene/Component.h"
#include "Math/Vector.h"
#include <variant>
#include <functional>

namespace axiom
{

    using FieldValue = std::variant<float, int, bool, Vec3, String>;

    enum class FieldType { Float, Int, Bool, Vec3, String };
    

    struct FieldDescriptor
    {
        const char* name;
        FieldType type;
        std::function<FieldValue(const void*)> get;
        std::function<void(void*, const FieldValue&)> set;

    };

    struct TypeDescriptor
    {
        const char* name = nullptr;
        std::type_index type = typeid(void);
        Vector<FieldDescriptor> fields;
        std::function<UniquePtr<Component>()> factory;
    };


    class TypeRegistry 
    {
        public:
            void Register(TypeDescriptor desc)
            {
                m_descriptors.insert_or_assign(desc.type, std::move(desc));
            }


            const TypeDescriptor* Find(std::type_index id) const
            {
                auto it = m_descriptors.find(id);
                if (it != m_descriptors.end())
                    return &it->second;
                else
                    return nullptr;
            }

            UniquePtr<Component> Create(const String& name) const
            {
                for(auto& [key, typeDescriptor] : m_descriptors)
                {
                    if(typeDescriptor.name == name)
                    {
                        return typeDescriptor.factory();
                    }
                }
                return nullptr;
            }
        private:
            TypeMap<TypeDescriptor> m_descriptors;
    };

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
        f.get  = [m](const void* o)                     { return FieldValue(static_cast<const T*>(o)->*m); };
        f.set  = [m](void* o, const FieldValue& v)      { static_cast<T*>(o)->*m = std::get<U>(v); };
        return f;
    }

    template<class T, class U>
    FieldDescriptor MakeField(const char* name, U (T::*getter)() const, void (T::*setter)(U))
    {
        FieldDescriptor f;
        f.name = name;
        f.type = MapFieldType<U>();
        f.get = [getter](const void* o)                { return FieldValue((static_cast<const T*>(o)->*getter)()); };
        f.set = [setter](void* o, const FieldValue& v) { (static_cast<T*>(o)->*setter)(std::get<U>(v)); };
        return f;
    }

    class Reflection
    {
    public:
        inline static const TypeRegistry& Get() { AX_ASSERT(s_instance, "No Reflection instance set."); return *s_instance;};
    private:
        inline static void SetInstance(TypeRegistry* instance) { s_instance = instance; }
        inline static TypeRegistry* s_instance = nullptr;
        friend class ReflectionSubsystem;
    };
}