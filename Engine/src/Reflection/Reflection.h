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

    class Archive;

    using FieldValue = std::variant<float, int, bool, Vec3, String>;

    enum class FieldType { Float, Int, Bool, Vec3, String, Opaque };

    struct FieldDescriptor
    {
        const char* name;
        FieldType type = FieldType::Opaque;
        std::function<FieldValue(const void*)> get;
        std::function<void(void*, const FieldValue&)> set;

        std::function<void(Archive&, const void*)> serialize;
        std::function<void(Archive&, void*)> deserialize;
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