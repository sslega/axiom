#type vertex
#version 330 core
#include "axiom_transform.glsl"
#include "axiom_debug_modes.glsl"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_LocalNormal;
out vec3 v_WorldNormal;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_LocalNormal = a_Normal;
    v_WorldNormal = normalize(transpose(mat3(u_WorldToLocal)) * a_Normal);

    gl_Position = u_ViewProjection * u_LocalToWorld * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core
#include "axiom_debug_modes.glsl"

layout(location = 0) out vec4 color;

in vec3 v_Position;
in vec2 v_TexCoord;
in vec3 v_LocalNormal;
in vec3 v_WorldNormal;

uniform int u_DebugMode;

void main()
{
    vec3 output = vec3(0,0,0);
    if(u_DebugMode == DEBUG_DRAW_WORLD_NORMAL)
        output = v_WorldNormal * 0.5 + 0.5;
        
    color = vec4(output, 1.0);
}