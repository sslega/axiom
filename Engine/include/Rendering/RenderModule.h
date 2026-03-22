#pragma once

#include "RenderDevice.h"
#include "Core/EngineModule.h"
#include "Rendering/RenderTypes.h"
#include "Rendering/RenderMeshProxy.h"


namespace axiom
{
    class RenderModule : public EngineModule
    {
    public:
        RenderModule(Application& engine);

        bool Initialize() override;
        void Shutdown() override;
        void Update() override;
        void Render() override;

    private:
        UniquePtr<IRenderDevice> m_renderer;
        RenderMeshProxy* GetProxy(SharedPtr<Mesh> mesh);
        RenderMeshProxy* CreateProxy(SharedPtr<Mesh> mesh);
        void DestroyProxy(SharedPtr<Mesh> mesh);

        std::unordered_map<SharedPtr<Mesh>, UniquePtr<RenderMeshProxy>> m_renderProxies;
    };
}