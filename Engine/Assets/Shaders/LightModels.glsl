#type vertex
#version 330 core
#include "axiom_transform.glsl"
#include "axiom_debug_modes.glsl"

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in vec4 a_Color;

out vec3 v_WorldPosition;
out vec4 v_Color;
out vec2 v_TexCoord;
out vec3 v_LocalNormal;
out vec3 v_WorldNormal;

void main()
{
    vec4 worldPos = u_LocalToWorld * vec4(a_Position, 1.0);
    v_WorldPosition = worldPos.xyz;
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_LocalNormal = a_Normal;
    v_WorldNormal = normalize(transpose(mat3(u_WorldToLocal)) * a_Normal);

    gl_Position = u_ViewProjection * worldPos;
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec3 v_WorldPosition;
in vec2 v_TexCoord;
in vec3 v_LocalNormal;
in vec3 v_WorldNormal;
in vec4 v_Color;

uniform vec3 u_LightDir;     // direction TO the light, world space
uniform vec3 u_LightColor;
uniform vec3 u_CameraPos;    // world space camera position
uniform float u_Roughness;   // 0 = mirror-smooth, 1 = fully rough

#define RECIPROCAL_PI 0.31830988618

float saturate(float v) { return clamp(v, 0.0, 1.0); }

#define COOK_TORRANCE

#ifdef LAMBERT
vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    vec3 diffuse =  albedo * max(dot(N, L), 0) * lightColor;
    return diffuse;
}
#endif

#ifdef PHONG
vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    vec3 diffuse = lightColor * max(dot(N, L), 0);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, V), 0), shininess * 10);
    return diffuse + specular;
}
#endif

#ifdef BLINN_PHONG
vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    vec3 diffuse = lightColor * max(dot(N, L), 0);
    vec3 H = normalize(L + V);
    float specular = pow(max(dot(N, H), 0), shininess * 10);
    return diffuse + specular;
}
#endif

#ifdef COOK_TORRANCE
#define PI 3.1415926535897932384626433832795

float D_GGX(float NdotH, float roughness) {
    float a  = roughness * roughness;   // Disney conversion: α = roughness²
    float a2 = a * a;
    float denom = NdotH * NdotH * (a2 - 1.0) + 1.0;
    return a2 / (PI * denom * denom);
}

vec3 F_Schlick(float VdotH, vec3 F0) {
    float f = pow(1.0 - VdotH, 5.0);
    return F0 + (1.0 - F0) * f;
}

float G_SchlickGGX(float NdotX, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;            // remap for direct light
    return NdotX / (NdotX * (1.0 - k) + k);
}

float G_Smith(float NdotV, float NdotL, float roughness) {
    return G_SchlickGGX(NdotV, roughness) * G_SchlickGGX(NdotL, roughness);
}

vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    float metallic = 0;
    float roughness = 1.0 - shininess;
    vec3 H = normalize(L + V);
    float NdotL = max(dot(N, L), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotH = max(dot(N, H), 0.0);
    float VdotH = max(dot(V, H), 0.0);

    vec3 F0 = vec3(1,1,1) * mix(0.04, 1.0, metallic);

    float D = D_GGX(NdotH, roughness); // Normal Distribution Function (GGX / Trowbridge-Reitz)
    vec3  F = F_Schlick(VdotH, F0); // F — Fresnel (Schlick)
    float G = G_Smith(NdotV, NdotL, roughness); // G — Geometry function (Smith + Schlick-GGX)

    // Diffuse with energy preservation
    vec3 kS = F;                              // how much specular used
    vec3 kD = (1.0 - kS) * (1.0 - metallic);  // metalls have no diffuse
    vec3 diffuse = kD * albedo / PI;
    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 1e-5);
    return (diffuse + specular) * lightColor * NdotL;
}
#endif

void main()
{
    vec3 N = normalize(v_WorldNormal);
    vec3 L = normalize(u_LightDir);
    vec3 V = normalize(u_CameraPos - v_WorldPosition);

    vec3 Albedo = vec3(0.8, 0.8, 0.8);
    float Shininess = 1.0 - u_Roughness;
    vec3 OutColor = CalculateLight(N, L, V, u_LightColor, Albedo, Shininess);

    color = vec4(OutColor, 1.0);
}
