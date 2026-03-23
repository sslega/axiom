#pragma once

#include "RenderDevice.h"
#include "Core/ApplicationModule.h"
#include "Rendering/RenderTypes.h"
#include "Rendering/RenderMesh.h"


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
        
        UniquePtr<IRenderDevice> m_renderer;
        RenderMesh* GetProxy(SharedPtr<MeshResource> mesh);
        RenderMesh* CreateProxy(SharedPtr<MeshResource> mesh);
        void DestroyProxy(SharedPtr<MeshResource> mesh);

        std::unordered_map<SharedPtr<MeshResource>, UniquePtr<RenderMesh>> m_renderProxies;
    };
}