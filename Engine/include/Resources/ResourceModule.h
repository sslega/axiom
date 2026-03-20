#pragma once

#include "Core/EngineModule.h"
#include "Core/Types.h"
#include "Resources/ResourceLoader.h"

namespace axiom
{
    enum class ResourceType  { Mesh, Texture, Shader };
    enum class ResourceState { Unloaded, Loading, Ready, Failed };

    // struct ResourceHandle 
    // {
    //     uint32 id = 0;

    //     bool IsValid() const { return id != 0; }
    //     bool operator==(const ResourceHandle&) const = default;

    //     static ResourceHandle Invalid() { return {0}; }
    // };

    // template<typename T>
    // struct TypedHandle 
    // {
    //     ResourceHandle handle;
    //     bool IsValid() const { return handle.IsValid(); }
    //     static TypedHandle FromRaw(ResourceHandle h) { return { h }; }
    // };

    // struct ResourceRecord 
    // {
    //     SharedPtr<void> data;      // the actual asset, type-erased
    //     String sourcePath;          // where it came from on disk
    //     ResourceType type;                  // enum: Mesh, Texture, Shader...
    //     ResourceState state;                // Unloaded, Loading, Ready, Failed

    //     template<typename T>
    //     T* As() { return static_cast<T*>(data.get()); }
    // };

    class ResourceModule : public EngineModule
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
}