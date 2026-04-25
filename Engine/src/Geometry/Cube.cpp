#include "Cube.h"

namespace axiom
{
    Cube::Cube()
    {
        auto v = [](float x, float y, float z, float u, float w) -> Vertex {
            Vertex vert;
            vert.m_position = { x, y, z };
            vert.m_color    = { 1.0f, 1.0f, 1.0f, 1.0f };
            vert.m_uv       = { u, w };
            return vert;
        };

        m_vertices = {
            // Front (+Z)
            v(-1,-1, 1,  0,0), v( 1,-1, 1,  1,0), v( 1, 1, 1,  1,1), v(-1, 1, 1,  0,1),
            // Back (-Z)
            v( 1,-1,-1,  0,0), v(-1,-1,-1,  1,0), v(-1, 1,-1,  1,1), v( 1, 1,-1,  0,1),
            // Right (+X)
            v( 1,-1, 1,  0,0), v( 1,-1,-1,  1,0), v( 1, 1,-1,  1,1), v( 1, 1, 1,  0,1),
            // Left (-X)
            v(-1,-1,-1,  0,0), v(-1,-1, 1,  1,0), v(-1, 1, 1,  1,1), v(-1, 1,-1,  0,1),
            // Top (+Y)
            v(-1, 1, 1,  0,0), v( 1, 1, 1,  1,0), v( 1, 1,-1,  1,1), v(-1, 1,-1,  0,1),
            // Bottom (-Y)
            v(-1,-1,-1,  0,0), v( 1,-1,-1,  1,0), v( 1,-1, 1,  1,1), v(-1,-1, 1,  0,1),
        };

        m_indices = {
             0, 1, 2,  2, 3, 0,   // Front
             4, 5, 6,  6, 7, 4,   // Back
             8, 9,10, 10,11, 8,   // Right
            12,13,14, 14,15,12,   // Left
            16,17,18, 18,19,16,   // Top
            20,21,22, 22,23,20,   // Bottom
        };
    }
}
