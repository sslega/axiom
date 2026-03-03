#include "Resources/Resource.h"

namespace axiom
{
    Resource::Resource()
    {
    }

    String& Resource::GetPath()
    {
        return m_Path;
    }

    bool Resource::LoadFromFile(String &path)
    {
        m_Path = path;
        std::uintmax_t fileSize = std::filesystem::file_size(path);

        m_RawData.reserve(fileSize);
        std::ifstream file(path, std::ios::binary);
        file.read(reinterpret_cast<char*>(m_RawData.data()), fileSize);
        if(!file)
        {
            return false;
        }
        file.close();
        ParseData();
        return true;        
    }
    const Vector<char>& Resource::GetRawData() const
    {
        return m_RawData;
    }
}