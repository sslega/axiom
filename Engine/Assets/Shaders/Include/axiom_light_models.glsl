#ifndef AXIOM_LIGHT_MODELS_GLSL
#define AXIOM_LIGHT_MODELS_GLSL

#define RECIPROCAL_PI 0.31830988618

float saturate(float v) { return clamp(v, 0.0, 1.0); }

#ifdef LAMBERT
vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    vec3 diffuse = albedo * max(dot(N, L), 0.0) * lightColor;
    return diffuse;
}
#endif

#ifdef PHONG
vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    vec3 diffuse = lightColor * max(dot(N, L), 0.0);
    vec3 R = reflect(-L, N);
    float specular = pow(max(dot(R, V), 0.0), shininess * 10.0);
    return diffuse + specular;
}
#endif

#ifdef BLINN_PHONG
vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    vec3 diffuse = lightColor * max(dot(N, L), 0.0);
    vec3 H = normalize(L + V);
    float specular = pow(max(dot(N, H), 0.0), shininess * 10.0);
    return diffuse + specular;
}
#endif

#ifdef COOK_TORRANCE
#define PI 3.1415926535897932384626433832795

float D_GGX(float NdotH, float roughness) {
    float a  = roughness * roughness;
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
    float k = (r * r) / 8.0;
    return NdotX / (NdotX * (1.0 - k) + k);
}

float G_Smith(float NdotV, float NdotL, float roughness) {
    return G_SchlickGGX(NdotV, roughness) * G_SchlickGGX(NdotL, roughness);
}

vec3 CalculateLight(vec3 N, vec3 L, vec3 V, vec3 lightColor, vec3 albedo, float shininess)
{
    float metallic  = 0.0;
    float roughness = 1.0 - shininess;
    vec3  H      = normalize(L + V);
    float NdotL  = max(dot(N, L), 0.0);
    float NdotV  = max(dot(N, V), 0.0);
    float NdotH  = max(dot(N, H), 0.0);
    float VdotH  = max(dot(V, H), 0.0);

    vec3 F0 = vec3(1.0, 1.0, 1.0) * mix(0.04, 1.0, metallic);

    float D = D_GGX(NdotH, roughness);
    vec3  F = F_Schlick(VdotH, F0);
    float G = G_Smith(NdotV, NdotL, roughness);

    vec3 kS = F;
    vec3 kD = (1.0 - kS) * (1.0 - metallic);
    vec3 diffuse  = kD * albedo / PI;
    vec3 specular = (D * G * F) / (4.0 * NdotV * NdotL + 1e-5);
    return (diffuse + specular) * lightColor * NdotL;
}
#endif

#endif
