// Template for full-screen passes (post processes, JFA steps, etc.).
// Provides the whole vertex stage (a clip-space quad, no transforms) and the
// fragment main(). The including file declares its own textures/uniforms and
// implements FragmentShader(), same as with axiom_fragment.glsl.
//
// Usage:
//   #include "axiom_fullscreen.glsl"
//
//   uniform sampler2D u_ScreenTexture;
//
//   void FragmentShader(in VertexInput input, out FragmentInput output)
//   {
//       output.Color = texture(u_ScreenTexture, input.TexCoord).rgb;
//   }

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

out vec2 v_TexCoord;

void main()
{
    v_TexCoord = a_TexCoord;
    // The full-screen quad is already in clip space, so no matrices are applied.
    gl_Position = vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

in vec2 v_TexCoord;

layout(location = 0) out vec4 OutColor;

struct VertexInput
{
    vec2 TexCoord;
};

struct FragmentInput
{
    vec3 Color;
    float Opacity;
};

void FragmentShader(in VertexInput input, out FragmentInput output);

void main()
{
    VertexInput input;
    input.TexCoord = v_TexCoord;

    FragmentInput output;
    output.Color = vec3(0, 0, 0);
    output.Opacity = 1.0;
    FragmentShader(input, output);
    OutColor = vec4(output.Color, output.Opacity);
}
