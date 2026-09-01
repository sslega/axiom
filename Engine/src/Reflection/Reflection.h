#pragma once
#include "Core/Types.h"
#include "Scene/Component.h"
#include "Assert.h"
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
        size_t offset;
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
}