#pragma once
#include "Component.h"
#include "Core/Types.h"

namespace axiom
{
    class Mesh;
    
    class MeshComponent: public Component
    {
    public:
        MeshComponent();
        void SetMesh(const SharedPtr<Mesh>& mesh);
        const SharedPtr<Mesh>& GetMesh() const;
    protected:
        SharedPtr<Mesh> m_mesh;
    };
}