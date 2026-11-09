#pragma once
#include "Core/Types.h"
#include "Resources/Resource.h"

namespace axiom
{
    class ShaderResource : public Resource
    {
    public:
        ShaderResource(const String& vertexSource, const String& fragmentSource, const Vector<String>& sourceMap);
        const String& GetVertexSource() const { return m_vertexSource; }
        const String& GetFragmentSource() const { return m_fragmentSource; }
        const Vector<String>& GetSourceMap() const { return m_sourceMap; }
    
    protected:
        String m_vertexSource;
        String m_fragmentSource;
        Vector<String> m_sourceMap;
    };

}
