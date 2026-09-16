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

    using FieldValue = std::variant<float, int, bool, Vec3, String, Color>;

    enum class FieldType { Float, Int, Bool, Vec3, String, Color, Resource };

    struct FieldDescriptor
    {
        const char* name;
        FieldType type;
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
        std::type_index baseType = typeid(void);

        const FieldDescriptor* FindField(const char* fieldName) const;
    };


    class TypeRegistry 
    {
        public:
            template<class T>
            Vector<const TypeDescriptor*> GetAllTypes() const
            {
                Vector<const TypeDescriptor*> result;
                for(auto& [key, desc] : m_descriptors)
                {
                    if (IsA(desc.type, typeid(T))) result.push_back(&desc);
                }
                return result;
            }
            void Register(TypeDescriptor desc);
            const TypeDescriptor* Find(std::type_index id) const;
            UniquePtr<Component> Create(const String& name) const;
            bool IsA(std::type_index derived, std::type_index base) const;

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