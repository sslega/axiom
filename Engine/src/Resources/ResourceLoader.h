#pragma once

#include <vector>
#include <fstream>
#include <filesystem>

#include "Core/Types.h"
#include "Resources/Resource.h"

namespace axiom
{
    class ResourceModule;
    
    struct FileData
    {
        Vector<uint8> buffer;
        Path path;
        size_t GetSize() const { return buffer.size(); }
        bool IsEmpty() const { return buffer.empty(); }
    };

    class ResourceLoader
    {
    public:
        ResourceLoader(const ResourceModule& resourceModule);
        virtual ~ResourceLoader() = default;
        
        SharedPtr<Resource> Load(const Path& path);
    
    protected:
        virtual SharedPtr<Resource> CreateResource(const FileData& fileData) = 0;
        FileData ReadFile(const Path& path);
        const ResourceModule& m_resourceModule;
    };
    
}