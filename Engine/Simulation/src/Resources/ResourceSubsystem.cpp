#include "ResourceSubsystem.h"
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Core/SubsystemHost.h"
#include "Core/FileSubsystem.h"
#include "Core/StringUtils.h"
#include "Resource.h"
#include <algorithm>

namespace axiom
{
    ResourceSubsystem::ResourceSubsystem(SubsystemHost& host)
    :ApplicationSubsystem(host)
    {
    }
    
    ResourceSubsystem::~ResourceSubsystem()
    {
        
    }

    Path ResourceSubsystem::Resolve(const String& virtualPath) const
    {
        return fileSystemModule->Resolve(virtualPath);
    }
    
    void ResourceSubsystem::OnInitialize()
    {
        fileSystemModule = &GetSubsystem<FileSubsystem>();
    }

    SharedPtr<void> ResourceSubsystem::ResolveResource(const String& virtualPath)
    {
        return LoadInternal(virtualPath);
    }

    SharedPtr<void> ResourceSubsystem::LoadInternal(const String& virtualPath)
    {
        const String key = ToLower(virtualPath);
        auto it = m_resources.find(key);
        if (it != m_resources.end())
        {
            return it->second;
        }

        Path physicalPath = Resolve(virtualPath);
            
        ResourceLoader* loader = GetLoader(physicalPath);

        SharedPtr<Resource> resource = loader->Load(physicalPath);
        resource->m_path = virtualPath;
        m_resources[key] = resource;
        return resource;
    }

    void ResourceSubsystem::Evict(const String& virtualPath)
    {
        m_resources.erase(ToLower(virtualPath));
    }

    Vector<String> ResourceSubsystem::GetExtensions(std::type_index resourceType) const
    {
        Vector<String> extensions;
        for (const auto& [extension, loader] : m_loaders)
        {
            if (loader->GetResourceType() == resourceType)
            {
                extensions.push_back(extension);
            }
        }
        std::sort(extensions.begin(), extensions.end());
        return extensions;
    }

    ResourceLoader* ResourceSubsystem::GetLoader(Path path)
    {
        String fileExtension = path.extension().string();
        auto it = m_loaders.find(fileExtension);
        AX_ASSERT(it != m_loaders.end(), "No loader registered for extension");
        return it->second.get();
    }
}