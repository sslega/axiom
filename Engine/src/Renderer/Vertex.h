#pragma once

#include "Math/Vector.h"
#include "Renderer/Buffer.h"

namespace axiom
{
    class Vertex
    {
    public:
        Vertex();
        Vertex(Vec3 position);
        Vertex(Vec3 position, Vec2 uv);
        Vertex(Vec3 position, Vec2 uv, Vec3 normal);
        Vertex(Vec3 position, Vec2 uv, Vec3 normal, Vec4 color);

        Vec3 m_position = {0, 0, 0};
        Vec2 m_uv = {0, 0};
        Vec3 m_normal = {0, 0, 0};
        Vec4 m_color = {0, 0, 0, 0};

        static BufferLayout GetLayout()
        {
            return {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float2, "a_TexCoord" },
                { ShaderDataType::Float3, "a_Normal" },
                { ShaderDataType::Float4, "a_Color"}
            };
        }
    };
}
