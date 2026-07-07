#include "axiom_vertex.glsl"

void VertexShader(inout VertexInput input)
{
}

#include "axiom_fragment.glsl"

#define COOK_TORRANCE
#include "axiom_light_models.glsl"

uniform vec3  u_LightDir;
uniform vec3  u_LightColor;
uniform vec3  u_CameraPos;
uniform float u_Roughness;

void FragmentShader(in VertexInput input, out FragmentInput output)
{
    vec3  N         = normalize(input.WorldNormal);
    vec3  L         = normalize(u_LightDir);
    vec3  V         = normalize(u_CameraPos - input.WorldPosition);
    vec3  Albedo    = vec3(0.8, 0.8, 0.8);
    float Shininess = 1.0 - u_Roughness;

    output.Color = CalculateLight(N, L, V, u_LightColor, Albedo, Shininess);
}