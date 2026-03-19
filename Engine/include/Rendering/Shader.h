#pragma once
namespace axiom
{
    class Shader
    {
    public:
        Shader();
        ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    };
}