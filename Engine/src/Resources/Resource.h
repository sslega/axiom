#pragma once
#include "Core/Types.h"
#include "Core/Object.h"

namespace axiom
{
    class Resource : public Object
    {
    public:
        const String& GetPath() const { return m_path; }
    private:
        friend class ResourceModule;
        String m_path;
    };

    
} // namespace axiom