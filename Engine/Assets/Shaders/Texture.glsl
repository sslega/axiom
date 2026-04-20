#type vertex
#version 330 core
#include "axiom_transform.glsl"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;

    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec4 v_Color;
in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
    // color = v_Color * u_Color;
    // vec3 uvColor = vec3(v_TexCoord.x, v_TexCoord.y, 0.0f);
    // color = vec4(uvColor, 1.0f);
    vec4 textureSample = texture(u_Texture, v_TexCoord);
    color = textureSample;
}