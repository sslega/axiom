#include "Triangle.h"

namespace axiom
{
    Triangle::Triangle()
    {
        Vertex v0, v1, v2;
        v0.m_position = {-0.5f, -0.5f, 0.0f}; 
        v0.m_color = {1.0f, 0.5f, 0.5f, 1.0f}; 
        v0.m_uv = {0.0f, 0.0f};
        
        v1.m_position = { 0.5f, -0.5f, 0.0f}; 
        v1.m_color = {0.5f, 1.0f, 0.5f, 1.0f}; 
        v1.m_uv = {1.0f, 0.0f};

        v2.m_position = { 0.0f,  0.5f, 0.0f}; 
        v2.m_color = {0.5f, 0.5f, 1.0f, 1.0f}; 
        v2.m_uv = {0.5f, 1.0f};
        m_vertices = {v0, v1, v2};
        m_indices  = {0, 1, 2};
    }
}