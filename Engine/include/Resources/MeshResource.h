#pragma once

#include "Resources/Resource.h"
#include "Core/Types.h"
#include "Rendering/Vertex.h"

namespace axiom
{
    class MeshResource : public Resource
    {
    public:
        MeshResource(const Path& path);
    protected:
        Vector<Vertex> m_vertices;
        Vector<uint16> m_indices;

        virtual void ParseData() override;
    };
}