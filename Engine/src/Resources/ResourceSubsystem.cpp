#include "ResourceSubsystem.h"
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Core/Application.h"
#include "Core/FileSubsystem.h"
#include "Resource.h"

namespace axiom
{
    ResourceSubsystem::ResourceSubsystem(Application& application)
    :ApplicationSubsystem(application)
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

    SharedPtr<void> ResourceSubsystem::LoadInternal(const String& virtualPath)
    {
        auto it = m_resources.find(virtualPath);
        if (it != m_resources.end())
        {
            return it->second;
        }

        Path physicalPath = Resolve(virtualPath);
            
        ResourceLoader* loader = GetLoader(physicalPath);

        SharedPtr<Resource> resource = loader->Load(physicalPath);
        resource->m_path = virtualPath;
        m_resources[virtualPath] = resource;
        return resource;
    }

    ResourceLoader* ResourceSubsystem::GetLoader(Path path)
    {
        String fileExtension = path.extension().string();
        auto it = m_loaders.find(fileExtension);
        AX_ASSERT(it != m_loaders.end(), "No loader registered for extension");
        return it->second.get();
    }
}