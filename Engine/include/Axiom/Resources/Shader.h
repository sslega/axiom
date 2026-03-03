#pragma once

#include "../Core/Types.h"
#include "Resource.h"

namespace axiom
{
    class Shader : public Resource
    {
    public:
        Shader() = default;

        const String& GetVertexShader();
        const String& GetFragmentShader();

    protected:
        virtual void ParseData() override;

    private:
        String m_resource;
        
        String m_VertexShader;
        String m_FragmentShader;
    };
}