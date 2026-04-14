#pragma once

#include "Renderer/RenderMesh.h"

namespace axiom
{
    class MeshResource;

    class OpenGLRenderMesh : public RenderMesh
    {
    public:
        OpenGLRenderMesh(const MeshResource& meshResource);
        ~OpenGLRenderMesh();

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
