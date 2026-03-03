#pragma once

#include "../Core/Types.h"
#include <vector>
#include <fstream>
#include <filesystem>

namespace axiom
{
    class Resource
    {
    public:
        Resource();
        virtual ~Resource() = default;

        String& GetPath();
        bool LoadFromFile(String& path);
        const Vector<char>& GetRawData() const;

    private:
        String m_Path;
        Vector<char> m_RawData;
    
    protected:
        virtual void ParseData() = 0;
    };
}