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
    private:
        void ParseSource(const String& source, String& vertexSource, String& fragmentSource) const;
    };
}