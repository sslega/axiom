#include "axiom_fullscreen.glsl"

uniform sampler2D u_CustomMaskTexture;

void FragmentShader(in VertexInput input, out FragmentInput output)
{
    ivec2 coordinates = ivec2(gl_FragCoord.xy);
    float customMaskSample = texelFetch(u_CustomMaskTexture, coordinates, 0).r;
    ivec2 maskSize  = textureSize(u_CustomMaskTexture, 0);
    vec2 UV = (vec2(coordinates) + 0.5) / vec2(maskSize);
    output.Color = vec3(UV * customMaskSample, 0);
}
