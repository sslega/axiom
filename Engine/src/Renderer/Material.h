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
        Material(const SharedPtr<Shader> shader);

        void Bind();
        void Unbind();
        void SetTexture(const String& name, const SharedPtr<Texture2D> texture, uint32 slot);

        template<typename T>
        void SetUniform(const String& name, const T& value)
        { 
            m_uniforms[name] = value;
        }

    private:
        SharedPtr<Shader> m_shader;
        using UniformValue = std::variant<int, Vec4, Matrix4>;
        StringMap<UniformValue> m_uniforms;
        StringMap<std::pair<SharedPtr<Texture2D>, uint32>> m_textures;
    };
} // namespace axiom
