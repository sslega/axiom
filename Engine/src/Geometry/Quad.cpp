#include "Quad.h"

namespace axiom
{
    Quad::Quad()
    {
        Vertex v0, v1, v2, v3;
        v0.m_position = {-1.0f, -1.0f, 0.0f}; 
        v0.m_color = {1.0f, 1.0f, 1.0f, 1.0f}; 
        v0.m_uv = {0.0f, 0.0f};
        
        v1.m_position = {1.0f, -1.0f, 0.0f}; 
        v1.m_color = {1.0f, 1.0f, 1.0f, 1.0f}; 
        v1.m_uv = {1.0f, 0.0f};

        v2.m_position = {1.0f,  1.0f, 0.0f}; 
        v2.m_color = {1.0f, 1.0f, 1.0f, 1.0f}; 
        v2.m_uv = {1.0f, 1.0f};

        v3.m_position = {-1.0f,  1.0f, 0.0f}; 
        v3.m_color = {1.0f, 1.0f, 1.0f, 1.0f}; 
        v3.m_uv = {0.0f, 1.0f};
        
        m_vertices = {v0, v1, v2, v3};
        m_indices  = {0, 1, 2, 2, 3, 0};
    }
}