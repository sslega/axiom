#include "Resources/Resource.h"

namespace axiom
{
    Resource::Resource(const Path& path)
    : m_Path(path)
    {
    }

    bool Resource::Load()
    {
        std::uintmax_t fileSize = std::filesystem::file_size(m_Path);

        m_RawData.reserve(fileSize);
        std::ifstream file(m_Path, std::ios::binary);
        file.read(reinterpret_cast<char*>(m_RawData.data()), fileSize);
        if(!file)
        {
            return false;
        }
        file.close();
        ParseData();
        return true;        
    }

    const Path& Resource::GetPath()
    {
        return m_Path;
    }

    const Vector<char>& Resource::GetRawData() const
    {
        return m_RawData;
    }
}