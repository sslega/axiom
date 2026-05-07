#pragma once
#include "Resources/ResourceLoader.h"

namespace axiom
{
    class RenderModule;

    class MaterialLoader : public ResourceLoader
    {
    public:
        MaterialLoader(const ResourceModule& resourceModule, RenderModule& renderModule);
    protected:
        SharedPtr<Resource> CreateResource(const FileData& fileData) override;
    private:
        RenderModule& m_renderModule;
    };
}
