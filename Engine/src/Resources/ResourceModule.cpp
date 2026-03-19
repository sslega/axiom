#include "Resources/ResourceModule.h"
#include "Core/Core.h"

namespace axiom
{
    ResourceModule::ResourceModule(Application& engine)
    :EngineModule(engine)
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

    SharedPtr<void> ResourceModule::LoadInternal(Path path)
    {
        auto it = m_resources.find(path.string());
        if (it != m_resources.end())
        {
            return it->second;
        }
            
        ResourceLoader* loader = GetLoader(path);

        SharedPtr<void> resource = loader->Load(path);
        m_resources[path.string()] = resource;
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