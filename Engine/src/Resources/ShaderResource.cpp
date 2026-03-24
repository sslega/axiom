#include "Resources/ShaderResource.h"

namespace axiom
{
    ShaderResource::ShaderResource(const String& vertexSource, const String& fragmentSource)
    : m_vertexSource(vertexSource)
    , m_fragmentSource(fragmentSource)
    {
    }

    const String& ShaderResource::GetVertexSource() const
    {
        return m_vertexSource;
    }

    const String& ShaderResource::GetFragmentSource() const
    {
        return m_fragmentSource;
    }

}

