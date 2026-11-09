#include "Platform/OpenGL/OpenGLRenderMesh.h"
#include "Resources/MeshResource.h"

namespace axiom
{
    OpenGLRenderMesh::OpenGLRenderMesh(const MeshResource& meshResource)
    {

    }

    OpenGLRenderMesh::~OpenGLRenderMesh()
    {
    }

    void OpenGLRenderMesh::Bind() const
    {
    }

    void OpenGLRenderMesh::Unbind() const
    {
    }

    uint32 OpenGLRenderMesh::GetIndexCount() const
    {
        return uint32();
    }
}
