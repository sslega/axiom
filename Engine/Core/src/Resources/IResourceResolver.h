#pragma once

#include "Core/Types.h"

namespace axiom
{
    /// Loads a resource by virtual path for code that can't see the resource system, such as Archive.
    class IResourceResolver
    {
    public:
        virtual ~IResourceResolver() = default;

        virtual SharedPtr<void> ResolveResource(const String& virtualPath) = 0;
    };
}
