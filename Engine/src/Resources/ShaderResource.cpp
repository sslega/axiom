#include "Resources/ShaderResource.h"

namespace axiom
{
    ShaderResource::ShaderResource(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap)
    : m_vertexSource(vertexSource)
    , m_fragmentSource(fragmentSource)
    , m_sourceMap(sourceMap)
    {
    }

}

