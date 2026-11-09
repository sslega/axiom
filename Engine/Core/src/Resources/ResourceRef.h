#pragma once

#include "Core/Types.h"
#include "Resources/IResourceResolver.h"

namespace axiom
{
    /// A component field that references a resource by path.
    ///
    /// It's saved as the path and resolved during scene load, before the component initializes.
    /// Until Resolve() fills in the resource, GetResource() is null.
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

        void Resolve(IResourceResolver& resolver)
        {
            if(!m_path.empty() && !IsResolved())
            {
                m_resource = std::static_pointer_cast<T>(resolver.ResolveResource(m_path));
            }
        }

    private:
        String m_path;
        SharedPtr<T> m_resource;
    };
}