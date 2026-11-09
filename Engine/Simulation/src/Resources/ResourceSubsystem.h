#pragma once

#include "Core/Types.h"
#include "Core/ApplicationSubsystem.h"
#include "Resources/IResourceResolver.h"
#include "Resources/ResourceLoader.h"
#include "Resources/ResourceRef.h"

namespace axiom
{
    class FileSubsystem;

    /// Loads assets by virtual path. Picks a loader by file extension and caches the result by path, ignoring case.
    ///
    /// Resources are CPU-side data, and the renderer creates GPU objects from them.
    /// The exception is materials, which hold a compiled shader.
    ///
    /// @code{.cpp}
    /// SharedPtr<MeshResource> mesh = resources.Load<MeshResource>("project://Meshes/house.obj");
    /// resources.RegisterLoader<OBJLoader>(".obj"); // the extension includes the dot
    /// @endcode
    ///
    /// Loading the same path again returns the cached object; Evict() forgets it.
    class ResourceSubsystem : public ApplicationSubsystem, public IResourceResolver
    {
    public:
        ResourceSubsystem(SubsystemHost& host);
        ~ResourceSubsystem();

        Path Resolve(const String& virtualPath) const;

        void Evict(const String& virtualPath);
        
        template<typename T>
        SharedPtr<T> Load(const String& virtualPath)
        {
            return std::static_pointer_cast<T>(LoadInternal(virtualPath));
        }
        

        template<typename T, typename... Args>
        void RegisterLoader(const String& fileExtension, Args&&... args)
        {
            static_assert(std::is_base_of_v<ResourceLoader, T>, "T must derive from ResourceLoader");
            m_loaders[fileExtension] = MakeUnique<T>(*this, std::forward<Args>(args)...);
        }

        /// The file extensions, with the dot and sorted, whose loader produces the given resource class.
        Vector<String> GetExtensions(std::type_index resourceType) const;

        SharedPtr<void> ResolveResource(const String& virtualPath) override;

    protected:
        void OnInitialize() override;
        
    private:
        FileSubsystem* fileSystemModule;

        StringMap<UniquePtr<ResourceLoader>> m_loaders;
        StringMap<SharedPtr<void>> m_resources;

        ResourceLoader* GetLoader(Path physicalPath);
        SharedPtr<void> LoadInternal(const String& virtualPath);
    };
};