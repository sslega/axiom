#include "axiom_vertex.glsl"

void VertexShader(inout VertexInput input)
{
}

#include "axiom_fragment.glsl"

uniform vec4 u_Color;

void FragmentShader(in VertexInput input, out FragmentInput output)
{
    output.Color = input.Color.rgb * u_Color.rgb;
    output.Opacity = input.Color.a * u_Color.a;
}
