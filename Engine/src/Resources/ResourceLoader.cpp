#include "Resources/ResourceLoader.h"
#include "Core/Assert.h"
#include "Core/FileSystemModule.h"

namespace axiom
{

    ResourceLoader::ResourceLoader()
    {
    }

    SharedPtr<void> ResourceLoader::Load(const Path& physicalPath)
    {
        FileData data = ReadFile(physicalPath);
        return CreateResource(data);
    }

    FileData ResourceLoader::ReadFile(const Path& physicalPath)
    {
        std::ifstream file(physicalPath, std::ios::binary | std::ios::ate);
        AX_ASSERT(file.is_open(), "Failed to open file");

        FileData data;
        data.path = physicalPath;
        data.buffer.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data.buffer.data()),data.buffer.size());
        file.close();
        return data;
    }
}