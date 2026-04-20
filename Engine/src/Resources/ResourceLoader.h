#pragma once

#include <vector>
#include <fstream>
#include <filesystem>

#include "Core/Types.h"

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
        
        SharedPtr<void> Load(const Path& path);
    
    protected:
        virtual SharedPtr<void> CreateResource(const FileData& fileData) = 0;
        FileData ReadFile(const Path& path);
        const ResourceModule& m_resourceModule;
    };
    
}