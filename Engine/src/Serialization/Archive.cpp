#include "Archive.h"
namespace axiom
{
    Archive::Archive(nlohmann::json &node, Vector<UniquePtr<IResolvable>>& handles)
    : m_node(node), m_handles(handles)
    {
    }

    void Archive::RegisterHandle(UniquePtr<IResolvable> handle)
    {
        m_handles.push_back(std::move(handle));
    }

    void Archive::MergeHandlesInto(Archive &other)
    {
        for (auto& handle : m_handles)
        {
            other.m_handles.push_back(std::move(handle));
        }
        m_handles.clear();
    }
} // namespace axiom
