#type fragment
#version 330 core

#ifndef AXIOM_FRAGMENT_GLSL
#define AXIOM_FRAGMENT_GLSL

in vec3 v_WorldPosition;
in vec2 v_TexCoord;
in vec4 v_Color;
in vec3 v_LocalNormal;
in vec3 v_WorldNormal;

layout(location = 0) out vec4 color;

struct VertexInput
{
    vec3 WorldPosition;
    vec2 TexCoord;
    vec4 Color;
    vec3 LocalNormal;
    vec3 WorldNormal;
};

struct FragmentInput
{
    vec3 Color;
    float Opacity;
};

void FragmentShader(in VertexInput input, out FragmentInput output);

void main()
{
    VertexInput input;
    input.WorldPosition = v_WorldPosition;
    input.TexCoord = v_TexCoord;
    input.Color = v_Color;
    input.LocalNormal = v_LocalNormal;
    input.WorldNormal = v_WorldNormal;

    FragmentInput output;
    output.Color = vec3(0,0,0);
    output.Opacity = 1.0;

    FragmentShader(input, output);
    
    color = vec4(output.Color, output.Opacity);
}

#endif
