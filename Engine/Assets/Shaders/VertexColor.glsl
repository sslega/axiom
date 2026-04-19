 #type vertex
 #version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
#ifdef INSTANCED
layout(location = 3) in vec4 a_InstanceTransform0;
layout(location = 4) in vec4 a_InstanceTransform1;
layout(location = 5) in vec4 a_InstanceTransform2;
layout(location = 6) in vec4 a_InstanceTransform3;
#else
uniform mat4 u_Transform;
#endif

out vec3 v_Position;
out vec4 v_Color;
out vec2 v_TexCoord;

void main()
{
    v_Position = a_Position;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;

    #ifdef INSTANCED
        mat4 transform = mat4(a_InstanceTransform0, a_InstanceTransform1,
                            a_InstanceTransform2, a_InstanceTransform3);
    #else
        mat4 transform = u_Transform;
    #endif

    gl_Position = u_ViewProjection * transform * vec4(a_Position, 1.0);
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
    color = v_Color * u_Color;
}