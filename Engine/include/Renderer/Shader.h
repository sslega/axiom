#pragma once

#include "Core/Types.h"
#include "Math/Matrix4.h"

namespace axiom
{
    class Shader
    {
    public:
        ~Shader() = default;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        
        virtual void UploadUniformMat4(const String& name, const Matrix4& matrix) = 0;
    private:
        uint32 m_rendererID;
    };
}