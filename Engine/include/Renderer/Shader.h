#pragma once

#include "Core/Types.h"

namespace axiom
{
    class Shader
    {
    public:
        ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
    private:
        uint32 m_rendererID;
    };
}