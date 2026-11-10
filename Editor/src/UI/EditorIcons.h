#pragma once

#include <typeindex>

namespace axiom
{
    class Entity;

    /// Decides which Font Awesome icon stands for a component or an entity. Returns a UTF-8 string for a label.
    ///
    /// @code{.cpp}
    /// const String row = String(EditorIcons::ForEntity(entity)) + "  " + entity.GetName();
    /// @endcode
    ///
    /// A new component type shows a generic icon until it gets a line in ForComponent().
    class EditorIcons
    {
    public:
        static const char* ForComponent(std::type_index type);
        static const char* ForEntity(const Entity& entity);
    };
}
