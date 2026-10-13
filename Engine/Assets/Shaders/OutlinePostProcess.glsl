#include "axiom_fullscreen.glsl"

uniform sampler2D u_ScreenTexture;
uniform sampler2D u_SeedTexture;
uniform vec4 u_SelectionColor;
uniform vec4 u_OutlineColor;
uniform int u_OutlineThickness;

void FragmentShader(in VertexInput input, out FragmentInput output)
{
    ivec2 seedTextureSize  = textureSize(u_SeedTexture, 0);
    ivec2 pixelCoords = ivec2(gl_FragCoord.xy);
    vec2 UV = (vec2(pixelCoords) + 0.5) / vec2(seedTextureSize);

    vec2 seedSample = texelFetch(u_SeedTexture, pixelCoords, 0).rg;
    ivec2 seedPixelCoors = ivec2(seedSample * vec2(seedTextureSize));
    vec3 screenColorSample = texelFetch(u_ScreenTexture, pixelCoords, 0).rgb;

    float d = length(vec2(seedPixelCoors) - vec2(pixelCoords));
    const float outlineWidthPx = 5;
    if(seedSample == vec2(0)) // No seed found
    {
        output.Color = screenColorSample;
        return;
    }
    if(seedPixelCoors == pixelCoords) // its own UV
    {
        output.Color = mix(screenColorSample, u_SelectionColor.rgb, u_SelectionColor.a);
        int edgeDitanceX = min(pixelCoords.x, seedTextureSize.x - pixelCoords.x);
        int edgeDitanceY = min(pixelCoords.y, seedTextureSize.y - pixelCoords.y);
        float edgeDistance = min(edgeDitanceX, edgeDitanceY);
        if(0 < edgeDistance && edgeDistance <= u_OutlineThickness)
            output.Color = mix(screenColorSample, u_OutlineColor.rgb, u_OutlineColor.a);
        return;
    }
    if(0 < d && d <= u_OutlineThickness) // Outline
    {
        output.Color = mix(screenColorSample, u_OutlineColor.rgb, u_OutlineColor.a);
        return;
    }

    output.Color = screenColorSample;
}
