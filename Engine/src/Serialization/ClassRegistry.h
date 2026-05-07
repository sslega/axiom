#pragma once
#include <functional>
#include "Core/Types.h"

namespace axiom
{
    class Component;

    using ComponentFactory = std::function<UniquePtr<Component>()>;

    class ClassRegistry
    {
    public:
        static ClassRegistry& Get();                               // singleton accessor
        void Register(const String& key, ComponentFactory factory);
        UniquePtr<Component> Create(const String& key);                // look up and call factory
    
    private:
        StringMap<ComponentFactory> m_factories;
    };

} // namespace axiom
