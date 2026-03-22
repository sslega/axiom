#include "Resources/ResourceModule.h"
#include "Core/Types.h"
#include "Core/Assert.h"
#include "Core/Application.h"
#include "Core/FileSystemModule.h"

namespace axiom
{
    ResourceModule::ResourceModule(Application& application)
    :EngineModule(application)
    {
        
    }
    
    ResourceModule::~ResourceModule()
    {
        
    }

    bool ResourceModule::Initialize()
{
        return true;
    }
    
    void ResourceModule::OnRegister()
    {
        
    }
    
    void ResourceModule::OnUnregister()
    {
        
    }
    
    void ResourceModule::Shutdown()
    {
        
    }

    void ResourceModule::Update()
    {
        
    }

    void ResourceModule::Render()
    {
        
    }

    void ResourceModule::RegisterLoader(const String & fileExtension, UniquePtr<ResourceLoader> loader)
    {
        m_loaders[fileExtension] = std::move(loader);
    }

    SharedPtr<void> ResourceModule::LoadInternal(const String& virtualPath)
    {
        auto it = m_resources.find(virtualPath);
        if (it != m_resources.end())
        {
            return it->second;
        }

        FileSystemModule* fileSystemModule = GetApp().GetModule<FileSystemModule>();
        Path physicalPath = fileSystemModule->Resolve(virtualPath);
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