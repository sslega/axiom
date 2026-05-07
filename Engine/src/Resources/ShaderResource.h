#pragma once
#include "Core/Types.h"
#include "Resources/Resource.h"

namespace axiom
{
    class ShaderResource : public Resource
    {
    public:
        ShaderResource(const String& vertexSource, const String& fragmentSource);
        const String& GetVertexSource() const;
        const String& GetFragmentSource() const;
    
    protected:
        String m_vertexSource;
        String m_fragmentSource;
    };

}
