#pragma once
#include "Rendering/RenderMeshProxy.h"
namespace axiom
{
    class GLRenderMeshProxy : public RenderMeshProxy
    {
        ~GLRenderMeshProxy();
    public:
        virtual void Upload(const Mesh& mesh) override;
        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual uint32 GetIndexCount() const override;
    private:
        uint32 m_vao = 0;
        uint32 m_vbo = 0;
        uint32 m_ibo = 0;
        uint32 m_indexCount = 0;
    };
}
