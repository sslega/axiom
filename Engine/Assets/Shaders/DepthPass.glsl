#type vertex
#version 330 core

#include "axiom_transform.glsl"

layout(location = 0) in vec3 a_Position;

void main()
{
    gl_Position = u_ViewProjection * u_LocalToWorld * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

void main()
{
}
