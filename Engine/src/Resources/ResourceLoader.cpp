#include "Resources/ResourceLoader.h"
#include "Core/Core.h"

namespace axiom
{

    ResourceLoader::ResourceLoader()
    {
    }

    SharedPtr<void> ResourceLoader::Load(const Path &path)
    {
        FileData data = ReadFile(path);
        return CreateResource(data);
    }

    FileData ResourceLoader::ReadFile(const Path &path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        AX_ASSERT(file.is_open(), "Failed to open file");

        FileData data;
        data.path = path;
        data.buffer.resize(file.tellg());
        file.seekg(0, std::ios::beg);
        file.read(reinterpret_cast<char*>(data.buffer.data()),data.buffer.size());
        file.close();
        return data;
    }
}