#pragma once

#include "RenderDevice.h"
#include "Core/ApplicationModule.h"
#include "Rendering/RenderTypes.h"
#include "Rendering/RenderMesh.h"
#include "Rendering/RenderShader.h"
#include "Resources/MeshResource.h"
#include "Resources/ShaderResource.h"

namespace axiom
{
    class SceneModule;
    class MeshComponent;

    class RenderModule : public ApplicationModule
    {
    public:
        RenderModule(Application& engine);

        bool Initialize() override;
        void Shutdown() override;
        void Update() override;
        void Render() override;

    private:
        SceneModule* m_sceneModule;

        Vector<MeshComponent*> m_meshComponents;
        
        RenderAPI m_renderApi;
        UniquePtr<IRenderDevice> m_renderer;

        RenderMesh* GetMesh(const SharedPtr<MeshResource> meshResource);
        UniquePtr<RenderMesh> CreateMesh(const SharedPtr<MeshResource> meshResource);
        void DestroyMesh(SharedPtr<MeshResource> meshResource);

        RenderShader* GetShader(const SharedPtr<ShaderResource> shaderResource);
        UniquePtr<RenderShader> CreateShader(const SharedPtr<ShaderResource> shaderResource);
        void DestroyShader(SharedPtr<ShaderResource> shaderResource);

        UnorderedMap<SharedPtr<MeshResource>, UniquePtr<RenderMesh>> m_meshes;
        UnorderedMap<SharedPtr<ShaderResource>, UniquePtr<RenderShader>> m_shaders;
    };
}