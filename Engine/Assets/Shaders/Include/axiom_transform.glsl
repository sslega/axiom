#ifndef AXIOM_TRANSFORM_GLSL
#define AXIOM_TRANSFORM_GLSL

#ifdef INSTANCED
layout(location = 3) in vec4 a_InstanceTransform0;
layout(location = 4) in vec4 a_InstanceTransform1;
layout(location = 5) in vec4 a_InstanceTransform2;
layout(location = 6) in vec4 a_InstanceTransform3;
#define u_Transform mat4(a_InstanceTransform0, a_InstanceTransform1, \
                         a_InstanceTransform2, a_InstanceTransform3)
#else
uniform mat4 u_Transform;
#endif

#endif