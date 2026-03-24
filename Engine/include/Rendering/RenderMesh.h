#pragma once
#include "Core/Types.h"
#include "Rendering/RenderMesh.h"
#include "Resources/MeshResource.h"

namespace axiom
{
    class RenderMesh
    {
    public:
        virtual ~RenderMesh() = default;
        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;
        virtual uint32 GetIndexCount() const = 0;
    };
}