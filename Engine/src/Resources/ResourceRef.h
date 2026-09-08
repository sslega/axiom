#pragma once

#include "Core/Types.h"

namespace axiom
{
    template<class T> 
    class ResourceRef
    {
    public:
        ResourceRef() = default;
        explicit ResourceRef(const String& path): m_path(path) {}

        bool IsResolved() const { return m_resource != nullptr; }

        const String& GetPath() const { return m_path; }
        const SharedPtr<T>& GetResource() const { return m_resource; }

        explicit operator bool() const { return IsResolved(); }
    
    private:
        String m_path;
        SharedPtr<T> m_resource;

        friend class ResourceSubsystem;
    };
}