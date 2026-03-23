#pragma once

#include "Core/Types.h"
#include "Core/ApplicationModule.h"
#include "Resources/ResourceLoader.h"

namespace axiom
{
    enum class ResourceType  { MeshResource, Texture, RenderShader };
    enum class ResourceState { Unloaded, Loading, Ready, Failed };

    class ResourceModule : public ApplicationModule
    {
    public:
        ResourceModule(Application& engine);
        ~ResourceModule();
        
        template<typename T>
        SharedPtr<T> Load(const String& virtualPath)
        {
            return std::static_pointer_cast<T>(LoadInternal(virtualPath));
        }
        

        void RegisterLoader(const String& fileExtension, UniquePtr<ResourceLoader> loader);

        virtual bool Initialize() override;
        virtual void OnRegister() override;
        virtual void OnUnregister() override;
        virtual void Shutdown() override;
        virtual void Update() override;
        virtual void Render() override;
    private:

        StringMap<UniquePtr<ResourceLoader>> m_loaders;
        StringMap<SharedPtr<void>> m_resources;

        ResourceLoader* GetLoader(Path physicalPath);
        SharedPtr<void> LoadInternal(const String& virtualPath);
    };
};