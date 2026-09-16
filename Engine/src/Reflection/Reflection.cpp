#include "Reflection.h"

namespace axiom
{
    void TypeRegistry::Register(TypeDescriptor desc)
    {
        if (const TypeDescriptor* base = Find(desc.baseType))
            desc.fields.insert(desc.fields.begin(), base->fields.begin(), base->fields.end());
        m_descriptors.insert_or_assign(desc.type, std::move(desc));
    }


    const TypeDescriptor* TypeRegistry::Find(std::type_index id) const
    {
        auto it = m_descriptors.find(id);
        if (it != m_descriptors.end())
            return &it->second;
        else
            return nullptr;
    }

    UniquePtr<Component> TypeRegistry::Create(const String &name) const
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
    
    bool TypeRegistry::IsA(std::type_index derived, std::type_index base) const
    {
        if(derived == base) return true;
        if(const TypeDescriptor* typeDescriptor = Find(derived))
        {
            return IsA(typeDescriptor->baseType, base);
        }
        return false;

    }
    
    const FieldDescriptor* TypeDescriptor::FindField(const char* fieldName) const
    {
        for (const auto& f : fields)
            if (std::strcmp(f.name, fieldName) == 0) return &f;
        return nullptr;
    }
}