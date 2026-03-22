#pragma once
#include "Core/Types.h"
#include "Rendering/Mesh.h"

namespace axiom
{
    class RenderMeshProxy
    {
    public:
        virtual ~RenderMeshProxy() = default;
        virtual void Upload(const Mesh& mesh) = 0;
        virtual void Bind()   const = 0;
        virtual void Unbind() const = 0;
        virtual uint32 GetIndexCount() const = 0;
    };
}