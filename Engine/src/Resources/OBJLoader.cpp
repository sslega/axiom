#include "OBJLoader.h"
#include "Math/Vector.h"
#include "Renderer/Vertex.h"
#include "Resources/MeshResource.h"
#include <sstream>
#include "Core/Log.h"
#include <regex>
#include <map>

namespace axiom
{
    OBJLoader::OBJLoader(const ResourceModule &resourceModule)
    : ResourceLoader(resourceModule)
    {
    }

    SharedPtr<void> OBJLoader::CreateResource(const FileData &fileData)
    {
        String source(fileData.buffer.begin(), fileData.buffer.end());

        Vector<Vec3> positions;
        Vector<Vec2> uvs;
        Vector<Vec3> normals;

        std::map<std::tuple<int,int,int>, uint32> indexMap;
        Vector<Vertex> vertices;
        Vector<uint32> indices;

        auto addVertex = [&](int posIdx, int uvIdx, int normalIdx)
        {
            auto key = std::make_tuple(posIdx, uvIdx, normalIdx);
            auto it = indexMap.find(key);
            if (it == indexMap.end())
            {
                Vertex v;
                v.m_position = positions[posIdx];
                v.m_uv       = uvs[uvIdx];
                v.m_normal   = normals[normalIdx];
                uint32 newIndex = (uint32)vertices.size();
                vertices.push_back(v);
                indexMap[key] = newIndex;
                indices.push_back(newIndex);
            }
            else
            {
                indices.push_back(it->second);
            }
        };

        std::istringstream stream(source);
        std::string line;
        while(std::getline(stream, line))
        {
            if (line.starts_with("v "))
            {
                std::regex vertex_regex("v (.*) (.*) (.*)");
                std::smatch match;
                if (std::regex_search(line, match, vertex_regex))
                {
                    double v1 = std::stod(match[1]);
                    double v2 = std::stod(match[2]);
                    double v3 = std::stod(match[3]);
                    positions.push_back(Vec3(v1, v2, v3));
                }
            }
            else if (line.starts_with("vt "))
            {
                std::regex vertex_regex("vt (.*) (.*)");
                std::smatch match;
                if (std::regex_search(line, match, vertex_regex))
                {
                    double v1 = std::stod(match[1]);
                    double v2 = std::stod(match[2]);
                    uvs.push_back(Vec2(v1, v2));
                }
            }
            else if (line.starts_with("vn "))
            {
                std::regex vertex_regex("vn (.*) (.*) (.*)");
                std::smatch match;
                if (std::regex_search(line, match, vertex_regex))
                {
                    double v1 = std::stod(match[1]);
                    double v2 = std::stod(match[2]);
                    double v3 = std::stod(match[3]);
                    normals.push_back(Vec3(v1, v2, v3));
                }
            }
            else if (line.starts_with("f "))
            {
                std::regex face_regex("f ([0-9]+)/([0-9]+)/([0-9]+) ([0-9]+)/([0-9]+)/([0-9]+) ([0-9]+)/([0-9]+)/([0-9]+)");
                std::smatch match;
                if (std::regex_search(line, match, face_regex))
                {
                    addVertex(std::stoi(match[1]) - 1, std::stoi(match[2]) - 1, std::stoi(match[3]) - 1);
                    addVertex(std::stoi(match[4]) - 1, std::stoi(match[5]) - 1, std::stoi(match[6]) - 1);
                    addVertex(std::stoi(match[7]) - 1, std::stoi(match[8]) - 1, std::stoi(match[9]) - 1);
                }
            }
        }
        return MakeShared<MeshResource>(vertices, indices);
    }
}