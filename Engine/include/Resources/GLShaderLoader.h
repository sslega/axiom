#pragma once

#include "Resources/ResourceLoader.h"

namespace axiom
{
    class GLShaderLoader : public ResourceLoader
    {
    public:
        GLShaderLoader() = default;
    
    protected:
        virtual SharedPtr<void> CreateResource(const FileData& fileData) override;
    };
}