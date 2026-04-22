#include "Resources/ResourceModule.h"
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Core/Application.h"
#include "Core/FileSystemModule.h"
#include "ResourceModule.h"

namespace axiom
{
    ResourceModule::ResourceModule(Application& application)
    :ApplicationModule(application)
    {
    }
    
    ResourceModule::~ResourceModule()
    {
        
    }

    Path ResourceModule::Resolve(const String& virtualPath) const
    {
        return fileSystemModule->Resolve(virtualPath);
    }
    
    void ResourceModule::OnInitialize()
    {
        fileSystemModule = &GetModule<FileSystemModule>();
    }

    SharedPtr<void> ResourceModule::LoadInternal(const String& virtualPath)
    {
        auto it = m_resources.find(virtualPath);
        if (it != m_resources.end())
        {
            return it->second;
        }

        Path physicalPath = Resolve(virtualPath);
        // Path physicalPath = 
            
        ResourceLoader* loader = GetLoader(physicalPath);

        SharedPtr<void> resource = loader->Load(physicalPath);
        m_resources[virtualPath] = resource;
        return resource;
    }

    ResourceLoader* ResourceModule::GetLoader(Path path)
    {
        String fileExtension = path.extension().string();
        auto it = m_loaders.find(fileExtension);
        AX_ASSERT(it != m_loaders.end(), "No loader registered for extension");
        return it->second.get();
    }
}