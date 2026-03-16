#pragma once

#include "Core/Types.h"
#include "Shader.h"
#include "Resources/GLShaderResource.h"

namespace axiom
{
    // class GLShader : public GLShaderResource
    class GLShader : public Shader
    {
    public:
        GLShader(const GLShaderResource& resource);
        ~GLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Compile() override;

        // const char* GetVertexShader() override;
        // const char* GetFragmentShader() override;

    protected:
        // virtual void ParseData() override;
        // const char* m_VertexShader;
        // const char* m_FragmentShader;

        // uint32_t m_ID;
    };
}