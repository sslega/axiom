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
        Vertex(Vec3 position, Vec4 color);
        Vertex(Vec3 position, Vec4 color, Vec2 uv);

        Vec3 m_position;
        Vec4 m_color;
        Vec2 m_uv;

        static BufferLayout GetLayout()
        {
            return {
                { ShaderDataType::Float3, "a_Position" },
                { ShaderDataType::Float4, "a_Color"    },
                { ShaderDataType::Float2, "a_TexCoord" }
            };
        }
    };
}
