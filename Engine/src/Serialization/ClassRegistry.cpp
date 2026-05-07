#include "ClassRegistry.h"
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Scene/Component.h"

namespace axiom
{

    ClassRegistry &axiom::ClassRegistry::Get()
    {
        static ClassRegistry instance;
        return instance;
    }
    
    void ClassRegistry::Register(const String &key, ComponentFactory factory)
    {
        m_factories[key] = factory;
    }
    
    UniquePtr<Component> ClassRegistry::Create(const String &key)
    {
        auto it = m_factories.find(key);
        AX_ASSERT(it != m_factories.end(), "Class factory not found!");
        return it->second();
    }
} // namespace axiom