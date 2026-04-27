#type vertex
#version 330 core
#include "axiom_transform.glsl"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec3 v_Normal;
out vec3 v_Pos;
out vec4 v_Color;

void main()
{
    v_Pos  = vec3(u_Transform * vec4(a_Position, 1.0));
    v_Normal   = mat3(u_Transform) * a_Normal;
    v_Color    = a_Color;

    gl_Position = u_ViewProjection * vec4(v_Pos, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_Normal;
in vec3 v_Pos;
in vec4 v_Color;

uniform vec3 u_LightDir;     // direction TO the light, world space
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;    // world space camera position
uniform float u_Roughness;   // 0 = mirror-smooth, 1 = fully rough

void main()
{
    float shininess = (1.0 - u_Roughness) * 255.0 + 1.0;

    vec3 N = normalize(v_Normal);
    vec3 L = normalize(u_LightDir);
    vec3 V = normalize(u_CameraPos - v_Pos);
    vec3 H = normalize(L + V);

    vec3 ambient  = 0.1 * u_LightColor;
    vec3 diffuse  = max(dot(N, L), 0.0) * u_LightColor;
    vec3 specular = pow(max(dot(N, H), 0.0), shininess) * u_LightColor;

    vec3 result = (ambient + diffuse + specular) * v_Color.rgb;
    color = vec4(result, v_Color.a);
    
}
