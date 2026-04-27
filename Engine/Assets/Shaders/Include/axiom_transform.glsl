#ifndef AXIOM_TRANSFORM_GLSL
#define AXIOM_TRANSFORM_GLSL

uniform mat4 u_ViewProjection;

#ifdef INSTANCED
layout(location = 4) in vec4 a_InstanceTransform0;
layout(location = 5) in vec4 a_InstanceTransform1;
layout(location = 6) in vec4 a_InstanceTransform2;
layout(location = 7) in vec4 a_InstanceTransform3;
layout(location = 8) in vec4 a_InstanceInverse0;
layout(location = 9) in vec4 a_InstanceInverse1;
layout(location = 10) in vec4 a_InstanceInverse2;
layout(location = 11) in vec4 a_InstanceInverse3;
#define u_LocalToWorld mat4(a_InstanceTransform0, a_InstanceTransform1, \
                            a_InstanceTransform2, a_InstanceTransform3)
#define u_WorldToLocal mat4(a_InstanceInverse0, a_InstanceInverse1, \
                            a_InstanceInverse2, a_InstanceInverse3)
#else
uniform mat4 u_LocalToWorld;
uniform mat4 u_WorldToLocal;
#endif

#endif