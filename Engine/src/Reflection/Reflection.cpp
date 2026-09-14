#include "Reflection.h"

namespace axiom
{
    Vector<const TypeDescriptor *> TypeRegistry::GetAllTypes() const
    {
        Vector<const TypeDescriptor *> results;
        for(auto& [id, typeDescriptor] : m_descriptors)
        {
            results.push_back(&typeDescriptor);
        }
        return results;
    }
}