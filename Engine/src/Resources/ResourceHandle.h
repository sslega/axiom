#pragma once

#include "Resources/ResourceModule.h"
#include "Core/Types.h"

namespace axiom
{
    enum ResourceState 
    {
        Unresolved, 
        Resolved, 
        Failed 
    };

    struct IResolvable {
        virtual void Resolve(ResourceModule& resourceModule) = 0;
        virtual ~IResolvable() = default;
    };

    template<typename T> 
    class ResourceHandle : public IResolvable
    {
    public:
        ResourceHandle(const String& path, SharedPtr<T>& target)
        : path(path)
        , m_target(target)
        , state(ResourceState::Unresolved)
        {
        };

        void Resolve(ResourceModule& resourceModule) override
        {
            m_target = resourceModule.Load<T>(path);
            state = m_target ? ResourceState::Resolved : ResourceState::Failed;
        }
        
        String path;
        ResourceState state;
    private:
        SharedPtr<T>& m_target;
    };
    
} // namespace axiom
