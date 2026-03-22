#pragma once

#include <vector>
#include <fstream>
#include <filesystem>

#include "Core/Types.h"

namespace axiom
{
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
        ResourceLoader();
        virtual ~ResourceLoader() = default;
        
        SharedPtr<void> Load(const Path& path);
    
    protected:
        virtual SharedPtr<void> CreateResource(const FileData& fileData) = 0;
        
    private:
        FileData ReadFile(const Path& path);
    };
    
}