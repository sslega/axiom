#pragma once

#include "Resources/Resource.h"
#include "Core/Types.h"

namespace axiom
{
    class GLShaderResource : public Resource
    {
    public:
        GLShaderResource(const Path& path);

        // virtual void Bind() const = 0;
        // virtual void Unbind() const = 0;
        // virtual void Compile() = 0;

        virtual const char* GetVertexShader() = 0;
        virtual const char* GetFragmentShader() = 0;

    protected:
        virtual void ParseData() override = 0;

    // private:
    //     String m_resource;
        
    //     String m_VertexShader;
    //     String m_FragmentShader;

    //     uint32_t m_ID;
    };
}