#type vertex

#version 330 core
#ifndef AXIOM_VERTEX_GLSL
#define AXIOM_VERTEX_GLSL

#include "axiom_transform.glsl"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec4 a_Color;

out vec3 v_WorldPosition;
out vec2 v_TexCoord;
out vec4 v_Color;
out vec3 v_LocalNormal;
out vec3 v_WorldNormal;


struct VertexInput
{
    vec3 WorldPosition;
    vec2 TexCoord;
    vec4 Color;
    vec3 LocalNormal;
    vec3 WorldNormal;
};

void VertexShader(inout VertexInput input);

void main()
{
    VertexInput input;

    input.WorldPosition = (u_LocalToWorld * vec4(a_Position, 1.0)).xyz;
    input.TexCoord      = a_TexCoord;
    input.Color         = a_Color;
    input.LocalNormal   = a_Normal;
    input.WorldNormal   = normalize(transpose(mat3(u_WorldToLocal)) * a_Normal);

    VertexShader(input);

    v_WorldPosition = input.WorldPosition;
    v_TexCoord = input.TexCoord;
    v_Color = input.Color;
    v_LocalNormal = input.LocalNormal;
    v_WorldNormal = input.WorldNormal;

    gl_Position = u_ViewProjection * vec4(input.WorldPosition, 1.0);
}

#endif
