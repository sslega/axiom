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
        virtual SharedPtr<Shader> GetVariant(const Vector<String>& defines) = 0;

        virtual void UploadUniform(const String& name, const int& value) = 0;
        virtual void UploadUniform(const String& name, const float& value) = 0;
        virtual void UploadUniform(const String& name, const Vec2& value) = 0;
        virtual void UploadUniform(const String& name, const Vec3& value) = 0;
        virtual void UploadUniform(const String& name, const Vec4& value) = 0;
        virtual void UploadUniform(const String& name, const Matrix4& value) = 0;       

    private:
        uint32 m_rendererID;
    };
}