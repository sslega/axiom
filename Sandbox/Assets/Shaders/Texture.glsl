#include "axiom_vertex.glsl"

void VertexShader(inout VertexInput input)
{
}

#include "axiom_fragment.glsl"

uniform sampler2D u_Texture;

void FragmentShader(in VertexInput input, out FragmentInput output)
{
    vec4 sample = texture(u_Texture, input.TexCoord);
    output.Color = sample.rgb;
    output.Opacity = sample.a;
}