#include "axiom_fullscreen.glsl"

uniform sampler2D u_SeedTexture;
uniform int u_StepSize;

void FragmentShader(in VertexInput input, out FragmentInput output)
{
    ivec2 SeedTextureSize  = textureSize(u_SeedTexture, 0);
    ivec2 pixelCoords = ivec2(gl_FragCoord.xy);
    vec2 UV = (vec2(pixelCoords) + 0.5) / vec2(SeedTextureSize);
    vec2 best = vec2(0.0);  
    float bestDist = 1e20;
    for (int y = -1; y <= 1; ++y)
    {
        for (int x = -1; x <= 1; ++x)
        {
            ivec2 p = pixelCoords + ivec2(x, y) * u_StepSize;
            if(p.x < 0 || p.x >= SeedTextureSize.x || p.y < 0 || p.y >= SeedTextureSize.y ) continue;

            vec2 seed = texelFetch(u_SeedTexture, p, 0).rg;
            if(seed == vec2(0)) continue;
            float d = length((seed - UV) * SeedTextureSize);
            if(d < bestDist)
            {
                best = seed;
                bestDist = d;
            }
        }
    }
    output.Color = vec3(best, 0);   
}
