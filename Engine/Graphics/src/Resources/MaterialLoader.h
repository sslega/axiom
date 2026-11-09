#pragma once
#include "Resources/ResourceLoader.h"

namespace axiom
{
    class RenderSubsystem;

    class MaterialLoader : public ResourceLoader
    {
    public:
        MaterialLoader(const ResourceSubsystem& resourceModule, RenderSubsystem& renderModule);
        std::type_index GetResourceType() const override;
    protected:
        SharedPtr<Resource> CreateResource(const FileData& fileData) override;
    private:
        RenderSubsystem& m_renderModule;
    };
}
