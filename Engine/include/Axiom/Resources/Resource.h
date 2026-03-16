#pragma once

#include "Axiom/Core/Types.h"
#include <vector>
#include <fstream>
#include <filesystem>

namespace axiom
{
    class Resource
    {
    public:
        Resource(const Path& path);
        virtual ~Resource() = default;

        const Path& GetPath();
        bool Load();
        const Vector<char>& GetRawData() const;

    private:
        const Path m_Path;
        Vector<char> m_RawData;
    
    protected:
        virtual void ParseData() = 0;
    };
}