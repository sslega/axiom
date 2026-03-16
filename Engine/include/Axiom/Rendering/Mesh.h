#pragma once

#include "Axiom/Resources/Resource.h"
#include "Axiom/Core/Types.h"
#include "Axiom/Rendering/Vertex.h"

namespace axiom
{
    class Mesh : public Resource
    {
    public:
        Mesh();
        Mesh(const Path& path);
    protected:
        Vector<Vertex> m_vertices;
        Vector<uint16> m_indices;

        virtual void ParseData() override;
    };
}