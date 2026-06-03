#include "axiom_vertex.glsl"

void VertexShader(inout VertexInput input)
{
}

#include "axiom_fragment.glsl"

#define COOK_TORRANCE
#include "axiom_light_models.glsl"

#ifdef HAS_DIRECTIONAL_LIGHT 
uniform vec3  u_LightDir;
uniform vec3  u_LightColor;
uniform sampler2D u_ShadowMap;
uniform mat4 u_LightViewProjection;
#endif

uniform vec3  u_CameraPos;
uniform float u_Roughness;

#ifdef HAS_DIRECTIONAL_LIGHT 
struct ShadowInput
{
    vec3  worldPosition;
    float NdotL;
};

float CalculateShadow(ShadowInput shadowInput)
{
    if (shadowInput.NdotL <= 0.0)
        return 0;

    float shadowBias = 0.001;
    float slopeScaleBias = max(shadowBias * (1.0 - shadowInput.NdotL), shadowBias * 0.01);

    vec3 lightSpaceNDC = (u_LightViewProjection * vec4(shadowInput.worldPosition, 1.0)).xyz;
    vec3 texCoords = (lightSpaceNDC * 0.5 + 0.5);
    vec2 texelSize = 1.0 / vec2(textureSize(u_ShadowMap, 0));
    float shadowFactor = 0;
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            vec2 sampleOffset = vec2(x,y) * texelSize;
            vec4 shadowSample = texture(u_ShadowMap, texCoords.xy + sampleOffset);
            shadowFactor += texCoords.z > shadowSample.r + slopeScaleBias ? 0.0 : 1.0;
        }
    }

    shadowFactor /= 9.0;
    
    return shadowFactor;
}
#endif

void FragmentShader(in VertexInput input, out FragmentInput output)
{
#ifdef HAS_DIRECTIONAL_LIGHT
    vec3  N         = normalize(input.WorldNormal);
    vec3  L         = normalize(u_LightDir);
    vec3  V         = normalize(u_CameraPos - input.WorldPosition);
    float NdotL     = dot(N, L);
    vec3  Albedo    = vec3(0.8, 0.8, 0.8);
    float Shininess = 1.0 - u_Roughness;

    ShadowInput shadowInput;
    shadowInput.NdotL = NdotL;
    shadowInput.worldPosition = input.WorldPosition;
    
    float shadowFactor = CalculateShadow(shadowInput);
    
    output.Color = shadowFactor * CalculateLight(N, L, V, u_LightColor, Albedo, Shininess);
#else
    output.Color = vec3(0.8, 0.8, 0.8);
#endif
}