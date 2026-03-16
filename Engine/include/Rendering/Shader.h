#pragma once
namespace axiom
{
    class Shader
    {
    public:
        Shader();

        virtual void Bind() const;
        virtual void Unbind() const;
        virtual void Compile();
    };
}