#include "axiom_vertex.glsl"

void VertexShader(inout VertexInput input)
{
}

#include "axiom_fragment.glsl"


void FragmentShader(in VertexInput input, out FragmentInput output)
{
    output.Color = vec3(0.8, 0.6, 0.8)
}