#type vertex
#version 330 core

#include "axiom_transform.glsl"

layout(location = 0) in vec3 a_Position;

uniform mat4 u_ViewProjection;

void main()
{
    gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

void main()
{
}
