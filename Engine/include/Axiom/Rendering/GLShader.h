#pragma once

#include "Shader.h"
#include "Axiom/Core/Types.h"
#include "Axiom/Resources/Resource.h"

namespace axiom
{
    class GLShader : public Shader
    {
    public:
        GLShader(const Path& path);
        ~GLShader();

        virtual void Bind() const override;
        virtual void Unbind() const override;
        virtual void Compile() override;

        const char* GetVertexShader() override;
        const char* GetFragmentShader() override;

    protected:
        virtual void ParseData() override;
        const char* m_VertexShader;
        const char* m_FragmentShader;

        uint32_t m_ID;
    };
}