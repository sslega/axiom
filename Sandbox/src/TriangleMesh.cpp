#include "TriangleMesh.h"
#include "Rendering/Vertex.h"
#include "Core/Types.h"

using namespace axiom;

TriangleMesh::TriangleMesh()
{
    Vertex v1 = Vertex();
    v1.m_position = Vec3(0.0f, 0.5f, 0.0f);
    v1.m_color = Vec4(1.0f, 0.0f, 0.0f, 1.0f);

    Vertex v2 = Vertex();
    v2.m_position = Vec3(0.5f, -0.5f, 0.0f);
    v2.m_color = Vec4(0.0f, 1.0f, 0.0f, 1.0f);

    Vertex v3 = Vertex();
    v3.m_position = Vec3(-0.5f, -0.5f, 0.0f);
    v3.m_color = Vec4(0.0f, 0.0f, 1.0f, 1.0f);

    m_vertices.insert(m_vertices.end(), {v1, v2, v3});
}