#pragma once

#include "Resources/ResourceLoader.h"

namespace axiom
{
    class Texture2DLoader : public ResourceLoader
    {
    public:
        Texture2DLoader(const ResourceModule& resourceModule);

    protected:
        SharedPtr<void> CreateResource(const FileData& fileData) override;
    };
}
