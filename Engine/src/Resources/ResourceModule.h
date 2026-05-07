#pragma once

#include "Core/Types.h"
#include "Core/ApplicationModule.h"
#include "Resources/ResourceLoader.h"

namespace axiom
{
    class FileSystemModule;

    class ResourceModule : public ApplicationModule
    {
    public:
        ResourceModule(Application& engine);
        ~ResourceModule();

        Path Resolve(const String& virtualPath) const;
        
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

    protected:
        void OnInitialize() override;
        
    private:
        FileSystemModule* fileSystemModule;

        StringMap<UniquePtr<ResourceLoader>> m_loaders;
        StringMap<SharedPtr<void>> m_resources;

        ResourceLoader* GetLoader(Path physicalPath);
        SharedPtr<void> LoadInternal(const String& virtualPath);
    };
};