#pragma once

#include "Resources/ResourceLoader.h"
#include "Resources/Resource.h"

namespace axiom
{
    class Texture2DLoader : public ResourceLoader
    {
    public:
        Texture2DLoader(const ResourceModule& resourceModule);

    protected:
        SharedPtr<Resource> CreateResource(const FileData& fileData) override;
    };
}
