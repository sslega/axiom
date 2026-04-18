#pragma once
#include "Core/Types.h"
#include "Math/Vector.h"
#include "Math/Matrix4.h"
#include <variant>

namespace axiom
{
    class Shader;
    class ShaderResource;
    class Texture2D;

    class Material
    {
    public:
        SharedPtr<Shader>         m_shader;
        SharedPtr<ShaderResource> m_shaderResource;
        SharedPtr<Texture2D>      m_texture;

        void Bind();
        void Unbind();

        template<typename T>
        void SetUniform(const String& name, const T& value)
        { 
            m_uniforms[name] = value;
        }

    private:
        using UniformValue = std::variant<int, Vec4, Matrix4>;
        StringMap<UniformValue> m_uniforms;
    };
} // namespace axiom
