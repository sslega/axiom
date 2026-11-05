#pragma once

#include "Core/Types.h"

namespace axiom
{
    /// A component field that references a resource by path.
    ///
    /// It's saved as the path and resolved during scene load, before the component initializes.
    /// Only ResourceSubsystem can fill in the resource; until then GetResource() is null.
    ///
    /// @code{.cpp}
    /// ResourceRef<MeshResource> mesh;                                 // the field, in MeshComponent
    /// desc.fields.push_back(MakeField("mesh", &MeshComponent::mesh)); // its registration, in EngineReflection.cpp
    /// if (mesh) { const SharedPtr<MeshResource>& data = mesh.GetResource(); }
    /// @endcode
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