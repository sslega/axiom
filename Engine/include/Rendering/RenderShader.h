#pragma once
namespace axiom
{
    class RenderShader
    {
    public:
        RenderShader();
        ~RenderShader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}