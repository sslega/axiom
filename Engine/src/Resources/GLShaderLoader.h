#pragma once

#include "Resources/ResourceLoader.h"
#include "Core/Types.h"

namespace axiom
{
    class GLShaderLoader : public ResourceLoader
    {
    public:
        GLShaderLoader(const ResourceModule& resourceModule);
    
    protected:
        virtual SharedPtr<void> CreateResource(const FileData& fileData) override;
    private:
        void ParseSource(const String& source, String& vertexSource, String& fragmentSource) const;
        void ResolveIncludes(String& source);

        UnorderedSet<String> m_includes;
    };
}